#include <stdio.h>
#include "pico/stdlib.h"
#include "keyPad.h"

#define ROWS 4
#define COLS 4
const uint8_t Row_Pins[ROWS] = {9, 8, 6, 5}; 
const uint8_t Col_Pins[COLS] = {4, 3, 2, 27}; 

// Mapeamento do teclado matricial
const char KEY_MAP[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
void keypad_init() {
    for (int i = 0; i < ROWS; i++) {
        gpio_init(Row_Pins[i]);
        gpio_set_dir(Row_Pins[i], GPIO_OUT);
        gpio_put(Row_Pins[i], 0);
    }
    for (int i = 0; i < COLS; i++) {
        gpio_init(Col_Pins[i]);
        gpio_set_dir(Col_Pins[i], GPIO_IN);
        gpio_pull_down(Col_Pins[i]);
    }
}
char read_keypad() {
    for (int row = 0; row < ROWS; row++) {
        gpio_put(Row_Pins[row], 1);

        for (int col = 0; col < COLS; col++) {
            if (gpio_get(Col_Pins[col])) {
                gpio_put(Row_Pins[row], 0);
                sleep_ms(20);
                return KEY_MAP[row][col];
            }
        }
        gpio_put(Row_Pins[row], 0);
    }
    return '\0';
}