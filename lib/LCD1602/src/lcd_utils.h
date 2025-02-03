#pragma once

#include <Arduino.h>
#include "LCD1602_internal.h"

void lcd_write(struct pin_mapping *pin_mappings, struct pin_data *pin_data);
void binary_to_D_array(uint8_t value, uint8_t *D_array);