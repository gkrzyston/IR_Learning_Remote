/*
 * menu.h
 *
 *  Created on: Dec 4, 2022
 *      Author: mcprisk
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_
#include <stdint.h>

typedef enum Menus{main_menu, settings_menu, device_menu
                    ,learn_device_menu,learn_command_menu,
                    awaiting_menu,xbee_menu} Menu;

#define MAIN_MENU_PAGE_COUNT 3
#define DEVICE_MENU_PAGE_COUNT 4

void display_menu(Menu menu, uint8_t page, uint8_t device);
void button_pressed(uint8_t button);
void refresh_menu();
#endif /* INC_MENU_H_ */
