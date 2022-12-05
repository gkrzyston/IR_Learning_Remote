/*
 * menu.c
 *
 *  Created on: Dec 4, 2022
 *      Author: mcprisk
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "menu.h"
#include "ssd1306.h"

static Menu current_menu;
static uint8_t current_page;

// Store Menu Commands
static char menu_commands[9][10];
static char cmd_buf[10];

void display_menu(Menu menu, uint8_t page) {
	if (!page) return;
	current_menu = menu;
	current_page = page;
	switch(menu) {
	case main_menu:
		if (page > MAIN_MENU_PAGE_COUNT) return;
		strcpy(cmd_buf, "Device ");
		char device_number[3];
		for (uint8_t i = 1; i <= 6; ++i) {
			itoa(i+(6*(current_page - 1)), device_number, 10);
			strcat(cmd_buf, device_number);
			strcpy(menu_commands[i - 1], cmd_buf);
			cmd_buf[7] = '\0';
		}
		strcpy(menu_commands[6], "Prev Page");
		strcpy(menu_commands[7], "Settings");
		strcpy(menu_commands[8], "Next Page");
		break;
	case settings_menu:
		break;
	case device_menu:
		break;
	}
	for (uint8_t i = 1; i <= 9; ++i) {
		erase_buffer();
		draw_string(menu_commands[i-1], white);
		update_display(i);
	}
}

void button_pressed(uint8_t button) {
	switch(current_menu) {
	case main_menu:
		if (!button || button > 9) return;
		switch(button) {
		case 7: // Previous
			display_menu(main_menu, (current_page != 1) ? (current_page - 1) : MAIN_MENU_PAGE_COUNT);
			break;
		case 8: // Settings
			display_menu(settings_menu, 1);
			break;
		case 9: // Next
			display_menu(main_menu, (current_page != MAIN_MENU_PAGE_COUNT) ? (current_page + 1) : 1);
			break;
		default: // Device Selected
			display_menu(device_menu, button*current_page);
			break;
		}

		break;
	default:
		break;
	}
}
