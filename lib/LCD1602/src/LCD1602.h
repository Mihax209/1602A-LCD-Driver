#pragma once

#include <Arduino.h>

void LCD_setup(uint8_t rs, uint8_t rw, uint8_t e, uint8_t *d);
void LCD_destroy(void);
bool LCD_init(void);
void LCD_test_write(void);
