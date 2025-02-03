#include <Arduino.h>
#include "LCD1602_internal.h"
#include "lcd_utils.h"

void send_init(struct pin_mapping *pin_mappings, struct pin_data *pin_data)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00110000, pin_data->D);

    lcd_write(pin_mappings, pin_data);
}

void send_clear_display(struct pin_mapping *pin_mappings, struct pin_data *pin_data)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00000001, pin_data->D);

    lcd_write(pin_mappings, pin_data);

    delay(4);  // This requires extra wait
}

void send_return_home(struct pin_mapping *pin_mappings, struct pin_data *pin_data)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00000010, pin_data->D);

    lcd_write(pin_mappings, pin_data);

    delay(4);  // This requires extra wait
}

void send_entry_mode_set(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool is_inc)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00000100, pin_data->D);
    pin_data->D[1] = is_inc ? 1 : 0;
    // NOTE bit 'S' is unused for now

    lcd_write(pin_mappings, pin_data);
}

void send_display_on_off(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool display_on, bool cursor_on, bool blink_on)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00001000, pin_data->D);
    pin_data->D[2] = display_on ? 1 : 0;
    pin_data->D[1] = cursor_on ? 1 : 0;
    pin_data->D[0] = blink_on ? 1 : 0;

    lcd_write(pin_mappings, pin_data);
}

void send_cursor_display_shift(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool display_shift, bool shift_right)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00010000, pin_data->D);
    pin_data->D[3] = display_shift ? 1 : 0;
    pin_data->D[2] = shift_right ? 1 : 0;

    lcd_write(pin_mappings, pin_data);
}

void send_function_set(struct pin_mapping *pin_mappings, struct pin_data *pin_data, bool is_8_bit, bool is_2_lines, bool is_10_dots)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00100000, pin_data->D);
    pin_data->D[4] = is_8_bit ? 1 : 0;
    pin_data->D[3] = is_2_lines ? 1 : 0;
    pin_data->D[2] = is_10_dots ? 1 : 0;

    lcd_write(pin_mappings, pin_data);
}

void send_set_CG_RAM_addr(struct pin_mapping *pin_mappings, struct pin_data *pin_data, uint8_t cg_addr)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b01000000 | cg_addr , pin_data->D);

    lcd_write(pin_mappings, pin_data);
}

void send_set_DD_RAM_addr(struct pin_mapping *pin_mappings, struct pin_data *pin_data, uint8_t ram_addr)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b10000000 | ram_addr, pin_data->D);

    lcd_write(pin_mappings, pin_data);
}

void send_write_data(struct pin_mapping *pin_mappings, struct pin_data *pin_data, uint8_t data)
{
    pin_data->RS = 1;
    pin_data->RW = 0;
    binary_to_D_array(data, pin_data->D);

    lcd_write(pin_mappings, pin_data);
}
