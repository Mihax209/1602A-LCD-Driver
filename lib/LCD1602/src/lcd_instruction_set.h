#pragma once

#include <Arduino.h>

void send_init(struct pin_mapping *pin_mappings, struct pin_data *pin_data);
void send_clear_display(struct pin_mapping *pin_mappings, struct pin_data *pin_data);
void send_return_home(struct pin_mapping *pin_mappings, struct pin_data *pin_data);
void send_entry_mode_set(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool is_inc);
void send_display_on_off(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool display_on, bool cursor_on, bool blink_on);
void send_cursor_display_shift(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool display_shift, bool shift_right);
void send_function_set(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool is_8_bit, bool is_2_lines, bool is_10_dots);
void send_set_CG_RAM_addr(struct pin_mapping *pin_mappings, struct pin_data *pin_data, uint8_t cg_addr);
void send_set_DD_RAM_addr(struct pin_mapping *pin_mappings, struct pin_data *pin_data, uint8_t ram_addr);
void send_write_data(struct pin_mapping *pin_mappings, struct pin_data *pin_data, uint8_t data);
