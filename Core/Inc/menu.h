/*
 * menu.h
 *
 *  Created on: Dec 4, 2022
 *      Author: mcprisk
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

typedef enum Menus{main_menu, settings_menu, device_menu} Menu;

#define MAIN_MENU_PAGE_COUNT 3

void display_menu(Menu menu, uint8_t page);
void button_pressed(uint8_t button);

#endif /* INC_MENU_H_ */
