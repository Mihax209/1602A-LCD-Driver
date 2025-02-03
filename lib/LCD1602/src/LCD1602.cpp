#include <Arduino.h>
#include "LCD1602.h"
#include "LCD1602_internal.h"
#include "lcd_instruction_set.h"

struct pin_mapping g_pin_mappings = {};
enum setup_state g_setup_state = UNINITIALIZED;
struct pin_data g_pin_data = {};  // to reduce stack overhead

// TODO how to elegantly change the d array to be safe size-wise?
void LCD_setup(uint8_t rs, uint8_t rw, uint8_t e, uint8_t d[LCD_DATA_PIN_COUNT])
{
    g_pin_mappings.RS = rs; pinMode(g_pin_mappings.RS, OUTPUT);
    g_pin_mappings.RW = rw; pinMode(g_pin_mappings.RW, OUTPUT);
    g_pin_mappings.E = e;   pinMode(g_pin_mappings.E, OUTPUT);

    pinMode(DBG_PIN, OUTPUT);

    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        g_pin_mappings.D[i] = d[i]; pinMode(g_pin_mappings.D[i], OUTPUT);
    }

    g_setup_state = SETUP_DONE;
}

void LCD_destroy(void)
{
    g_setup_state = UNINITIALIZED;
    memset((void *)&g_pin_mappings, 0, sizeof(g_pin_mappings));
}

// TODO add init options
bool LCD_init(void)
{
    if (g_setup_state != SETUP_DONE)
    {
        return false;
    }

    /* following init diagram from datasheet */
    delay(30);
    send_init(&g_pin_mappings, &g_pin_data);
    delay(7);
    send_init(&g_pin_mappings, &g_pin_data);
    delay(2);
    send_init(&g_pin_mappings, &g_pin_data);

    /* second part of init diagram */
    send_function_set(&g_pin_mappings, &g_pin_data, true, true, false);
    send_display_on_off(&g_pin_mappings, &g_pin_data, false, false, false);
    send_clear_display(&g_pin_mappings, &g_pin_data);
    send_entry_mode_set(&g_pin_mappings, &g_pin_data, true);

    g_setup_state = INITIALIZED;
    return true;
}

void LCD_test_write(void)
{
    /* 2 lines */
    send_function_set(&g_pin_mappings, &g_pin_data, true, true, false);
    /* turn off display */
    send_display_on_off(&g_pin_mappings, &g_pin_data, false, false, false);
    /* display on */
    send_display_on_off(&g_pin_mappings, &g_pin_data, true, true, true);
    send_entry_mode_set(&g_pin_mappings, &g_pin_data, true);

    /* S */
    send_write_data(&g_pin_mappings, &g_pin_data, 0b01010011);

    /* DEC */
    send_write_data(&g_pin_mappings, &g_pin_data, 0b01000100);
    send_write_data(&g_pin_mappings, &g_pin_data, 0b01000101);
    send_write_data(&g_pin_mappings, &g_pin_data, 0b01000011);

    /* Second line */
    send_set_DD_RAM_addr(&g_pin_mappings, &g_pin_data, 0b1000000);

    /* CR */
    send_write_data(&g_pin_mappings, &g_pin_data, 0b01000011);
    send_write_data(&g_pin_mappings, &g_pin_data, 0b01010010);
}
