#include "ssd1306.h"

static uint8_t display_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
static uint8_t i2c_buffer[3] = {IO_CTRL_REG_1, 0xFF, 0x01};

static uint8_t init_display_cmds[] = {
		0xAE, // Turn Display Off to Sleep Mode
		0xD5, // Set Display Clock Divide
		0x80, // Default Oscillator Frequency and Divide Ratio
		0xA8, // Set Multiplex Ratio
		0x1F, // 32 MUX
		0xD3, // Set Display Offset
		0x00, // No Vertical Display Offset
		0x40, // Set Display Start Line to Zero
		0x8D, // Set the Charge Pump Status
		0x14, // Enable the Charge Pump
		0x20, // Set Memory Addressing Mode
		0x00, // Horizontal Addressing Mode
		0xA1, // Set Segment Re-map (Column Address 127 = SEG0)
		0xC8, // Set COM Output Scan Direction to remapped mode.
		0xDA, // Set COM Pins
		0x02, // Sequential Pin Configuration, disable L/R remap.
		0x81, // Set Contrast Control
		0x8F, // Slightly Higher than Default Contrast
		0xD9, // Set Pre-charge Period
		0xF1, // Maximum Phase 2 Period, Lower Phase 1 Period
		0xDB, // Set VCOMH Deselect Level
		0x40, // ~0.89*VCC
		0xA4, // Turn on Display, Resuming to RAM Content
		0xA6, // Set Normal Display Colors
		0x2E, // Deactivate Scrolling
		0xAF, // Turn Display to Normal Mode
};

static uint8_t init_draw_cmds[] = {
		0x22, // Set Page Address
		0x00, // Start Address
		0xFF, // End Address
		0x21, // Set Column Address
		0x00, // Column Start Address
		0x7F, // Column End Address
};

extern I2C_HandleTypeDef hi2c1;

#define I2C_SEND() HAL_I2C_Master_Transmit(&hi2c1, PX2_ADDR, i2c_buffer, 3, HAL_MAX_DELAY);

void disable_OLED_EEPROM_writes(void) {
	uint8_t buf[] = {0xF4, 0x01};
	HAL_I2C_Master_Transmit(&hi2c1, PX2_ADDR, buf, 2, HAL_MAX_DELAY);
}

void drop_all_CS(void) {
	i2c_buffer[1] = 0x00;
	i2c_buffer[2] = 0x00;
	I2C_SEND();
	HAL_Delay(1);
}

void raise_all_CS(void) {
	i2c_buffer[1] = 0xFF;
	i2c_buffer[2] = 0x01;
	I2C_SEND();
}

// Only one CS can be dropped at a time.
void drop_CS(uint8_t display) {
	// Set all high
	i2c_buffer[1] = 0xFF;
	i2c_buffer[2] = 0x01;
	// Set the specified display line low
	if (display == 9) {
		i2c_buffer[2] &= ~(0b1);
	} else if (display <= 8) {
		i2c_buffer[1] &= ~(0b1 << (display - 1));
	}
	I2C_SEND();
	HAL_Delay(1);
}

// Initializes the display on startup
void init_displays(void) {
	STOP_BUTTON_POLL();
	disable_OLED_EEPROM_writes();
	raise_all_CS();
	DC_HIGH();
	RST_HIGH();
	HAL_Delay(10);
	RST_LOW();
	HAL_SPI_Transmit(&hspi2, (uint8_t*) &init_display_cmds, 1, HAL_MAX_DELAY);
	HAL_Delay(10);
	RST_HIGH();

	DC_LOW();
	HAL_Delay(1);

	drop_all_CS();
	for (uint8_t j = 0; j < 80; ++j) asm("");

	// Send commands
	for (uint16_t i = 0; i < sizeof(init_display_cmds); ++i) {
		HAL_SPI_Transmit(&hspi2, (uint8_t *) &init_display_cmds[i], 1, HAL_MAX_DELAY);
		for (uint8_t j = 0; j < 10; ++j) asm("");
	}

	raise_all_CS();

	// Raise D/C# and clear all displays
	DC_HIGH();
	START_BUTTON_POLL();
	erase_buffer();
	update_all_displays();
}

// refreshes a display (1 indexed, left-to-right, top-to-bottom)
void update_display(uint8_t display) {
	STOP_BUTTON_POLL();
	DC_LOW();
	HAL_Delay(1);

	// Drop Chip Select Low
	drop_CS(display);

	for (uint8_t j = 0; j < 80; ++j) asm("");

	for (uint16_t i = 0; i < sizeof(init_draw_cmds); ++i) {
		HAL_SPI_Transmit(&hspi2, (uint8_t *) &init_draw_cmds[i], 1, HAL_MAX_DELAY);
		for (uint8_t j = 0; j < 10; ++j) asm("");
	}

	DC_HIGH();
	for (uint8_t j = 0; j < 80; ++j) asm("");

	for (uint16_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; ++i) {
		HAL_SPI_Transmit(&hspi2, (uint8_t *) &display_buffer[i], 1, HAL_MAX_DELAY);
		for (uint8_t k = 0; k < 10; ++k) asm("");
	}

	raise_all_CS();
	START_BUTTON_POLL();
}

void update_all_displays() {
	STOP_BUTTON_POLL();
	DC_LOW();
	HAL_Delay(1);
	drop_all_CS();

	for (uint8_t j = 0; j < 80; ++j) asm("");

	for (uint16_t i = 0; i < sizeof(init_draw_cmds); ++i) {
		HAL_SPI_Transmit(&hspi2, (uint8_t *) &init_draw_cmds[i], 1, HAL_MAX_DELAY);
		for (uint8_t j = 0; j < 10; ++j) asm("");
	}

	DC_HIGH();
	for (uint8_t j = 0; j < 80; ++j) asm("");

	for (uint16_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; ++i) {
		HAL_SPI_Transmit(&hspi2, (uint8_t *) &display_buffer[i], 1, HAL_MAX_DELAY);
		for (uint8_t k = 0; k < 10; ++k) asm("");
	}
	raise_all_CS();
	START_BUTTON_POLL();
}

// flips all bits in the buffer
void invert_buffer(void) {
	for (uint16_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; ++i) {
		display_buffer[i] = ~display_buffer[i];
    }
}

// writes zeroes to all bits in the buffer
void erase_buffer(void) {
	for (uint16_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; ++i) {
		display_buffer[i] = 0x00;
	}
}

// writes ones to all bits in the buffer
void fill_buffer(void) {
	for (uint16_t i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; ++i) {
		display_buffer[i] = 0xFF;
	}
}

// Draw a pixel to the buffer
// Adapted from https://github.com/afiskon/stm32-ssd1306/blob/master/ssd1306/ssd1306.c
void draw_pixel(uint8_t x, uint8_t y, Color color) {
    // If the pixel is outside of buffer bounds, break.
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;

    // Clear the bit of the buffer, then write a 0 or 1
    // Depending on the selected color.
	if (color) {
		display_buffer[x + (y / 8) * DISPLAY_WIDTH] |= (1 << (y & 7));
	} else {
		display_buffer[x + (y / 8) * DISPLAY_WIDTH] &= ~(1 << (y & 7));
	}
}

// Draw a character to the buffer
// x,y should correspond to top left corner of text
// Adapted from https://github.com/afiskon/stm32-ssd1306/blob/master/ssd1306/ssd1306.c
void draw_char(char ch, uint8_t x, uint8_t y, Color color) {
    if (ch < 32 || ch > 126) return;
    draw_bmp(&font[54*(ch - 32)], x, y, FONT_WIDTH, FONT_HEIGHT, color);
}

// Draw a bitmap to the buffer
// Adapted from https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.cpp
void draw_bmp(uint8_t* bmp, uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color) {
	int16_t byteWidth = (w + 7) / 8;
	uint8_t b = 0;
	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < w; i++) {
			if (i & 7)
				b <<= 1;
			else
				b = bmp[j * byteWidth + i / 8];
			draw_pixel(x + i, y, (b & 0x80) ? color : !color);
		}
	}
}

// Draw string centered on display
// maximum size that will fit on display: 10 characters
void draw_string(char* str, Color color) {
	size_t len = strlen(str);
    if (len <= 0 || len > 10) return;

    uint8_t x = DISPLAY_WIDTH/2;
    if (len%2) {
        x -= FONT_WIDTH/2;
        len -= 1;
    }
    x -= FONT_WIDTH * len/2;

    while(*str) {
        draw_char(*str, x, DISPLAY_HEIGHT/2 - FONT_HEIGHT/2, color);
        x += FONT_WIDTH;
        ++str;
    }
}
