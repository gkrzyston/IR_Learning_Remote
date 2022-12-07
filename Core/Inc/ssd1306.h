#ifndef SSD_1306
#define SSD_1306

#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "font.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define FONT_HEIGHT 27
#define FONT_WIDTH 12

extern SPI_HandleTypeDef hspi2;

typedef enum Colors{black = 0x00, white = 0x01} Color;

#define OLED_DC_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_0
#define OLED_RST_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_1
#define OLED_CS_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_4

#define RST_LOW() HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_RESET)
#define RST_HIGH() HAL_GPIO_WritePin(OLED_RST_Port, OLED_RST_Pin, GPIO_PIN_SET)
#define CS_LOW() HAL_GPIO_WritePin(OLED_CS_Port, OLED_CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(OLED_CS_Port, OLED_CS_Pin, GPIO_PIN_SET)
#define DC_LOW() HAL_GPIO_WritePin(OLED_DC_Port, OLED_DC_Pin, GPIO_PIN_RESET)
#define DC_HIGH() HAL_GPIO_WritePin(OLED_DC_Port, OLED_DC_Pin, GPIO_PIN_SET)

#define TIM5_CR1 0x40000C00
#define STOP_BUTTON_POLL() *(uint32_t*)TIM5_CR1 &= ~(0b1)
#define START_BUTTON_POLL() *(uint32_t*)TIM5_CR1 |= 0b1

// I2C Port Expander Addresses
#define PX2_ADDR      0xA2
#define IO_CTRL_REG_1 0xF2
#define IO_CTRL_REG_2 0xF3

/* Display Functions */

// Initializes the display on startup
void init_displays(void);
// refreshes the display
void update_display(uint8_t display);
// refreshes all displays with the current buffer
void update_all_displays();

/* Buffer Functions */
// flips all bits in the buffer
void invert_buffer(void);
// writes zeroes to all bits in the buffer
void erase_buffer(void);
// writes ones to all bits in the buffer
void fill_buffer(void);
// draw a high or low pixel to the buffer
void draw_pixel(uint8_t x, uint8_t y, Color color);

void draw_bmp(uint8_t* bmp, uint8_t x, uint8_t y, uint8_t w, uint8_t h, Color color);

/* Character Functions */
// Draw character
// x,y should correspond to top left corner of text
void draw_char(char c, uint8_t x, uint8_t y, Color color);
// Draw string centered on display
// maximum string size that will fit on display: 4 characters
void draw_string(char* str, Color color);

#endif // !SSD_1306
