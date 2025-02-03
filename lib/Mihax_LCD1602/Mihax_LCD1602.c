#include <Arduino.h>
#include "Mihax_LCD1602.h"

// TODO how to setup debug pin?
#define DBG_PIN (13)

struct pin_mapping {
    uint8_t RS;
    uint8_t RW;
    uint8_t E;
    uint8_t D[LCD_DATA_PIN_COUNT];
};

struct pin_data {
    uint8_t RS;
    uint8_t RW;
    uint8_t D[LCD_DATA_PIN_COUNT];
};

enum setup_state {
    UNINITIALIZED,
    SETUP_DONE,
    INITIALIZED
};

/* GLOBALS */
struct pin_mapping g_pin_mappings = {};
enum setup_state g_setup_state = UNINITIALIZED;
struct pin_data g_pin_data = {};  // to reduce stack overhead

inline uint8_t get_pin_value(uint16_t num, int idx)
{
    int mask_value = num & (1 << idx);
    return (mask_value != 0) ? HIGH : LOW;
}

void DBG(uint8_t val)
{
    digitalWrite(LED_BUILTIN, val);
    digitalWrite(DBG_PIN, val);
}

void lcd_pins_off(void)
{
    digitalWrite(g_pin_mappings.RS, LOW);
    digitalWrite(g_pin_mappings.RW, LOW);

    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        digitalWrite(g_pin_mappings.D[i], LOW);
    }
}

void lcd_write(struct pin_data *pin_data)
{
    /**
     * NOTE since this function is always used with the g_pin_data global,
     * we technically don't need a pointer to a pin_data struct. However, I think
     * it's more elegant to have a pointer given anyway, since otherwise it would be
     * easier to potentially call this function with an uninitialized g_pin_data struct
     */
    DBG(HIGH);

    digitalWrite(g_pin_mappings.RS, pin_data->RS);
    digitalWrite(g_pin_mappings.RW, pin_data->RW);

    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        digitalWrite(g_pin_mappings.D[i], pin_data->D[i]);
    }

    digitalWrite(g_pin_mappings.E, HIGH);
    delayMicroseconds(1);
    digitalWrite(g_pin_mappings.E, LOW);
    lcd_pins_off();
    delayMicroseconds(90);

    DBG(LOW);
}

void binary_to_D_array(uint8_t value, uint8_t *D_array)
{
    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        /* NOTE MSb is the last array entry, i.e. D7 == D[7] */
        uint8_t mask = (1 << i) & value;
        D_array[i] = (mask == 0) ? 0 : 1;
    }
}

void send_init(struct pin_data *pin_data)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00110000, pin_data->D);

    lcd_write(pin_data);
}

void send_clear_display(struct pin_data *pin_data)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00000001, pin_data->D);

    lcd_write(pin_data);

     delay(4);  // This requires extra wait
}

void send_return_home(struct pin_data *pin_data)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00000010, pin_data->D);

    lcd_write(pin_data);

    delay(4);  // This requires extra wait
}

void send_entry_mode_set(struct pin_data *pin_data, bool is_inc)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00000100, pin_data->D);
    pin_data->D[1] = is_inc ? 1 : 0;
    // NOTE bit 'S' is unused for now

    lcd_write(pin_data);
}

void send_display_on_off(struct pin_data *pin_data, bool display_on, bool cursor_on, bool blink_on)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00001000, pin_data->D);
    pin_data->D[2] = display_on ? 1 : 0;
    pin_data->D[1] = cursor_on ? 1 : 0;
    pin_data->D[0] = blink_on ? 1 : 0;

    lcd_write(pin_data);
}

void send_cursor_display_shift(struct pin_data *pin_data, bool display_shift, bool shift_right)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00010000, pin_data->D);
    pin_data->D[3] = display_shift ? 1 : 0;
    pin_data->D[2] = shift_right ? 1 : 0;

    lcd_write(pin_data);
}

void send_function_set(struct pin_data *pin_data, bool is_8_bit, bool is_2_lines, bool is_10_dots)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b00100000, pin_data->D);
    pin_data->D[4] = is_8_bit ? 1 : 0;
    pin_data->D[3] = is_2_lines ? 1 : 0;
    pin_data->D[2] = is_10_dots ? 1 : 0;

    lcd_write(pin_data);
}

void send_set_CG_RAM_addr(struct pin_data *pin_data, uint8_t cg_addr)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b01000000 | cg_addr , pin_data->D);

    lcd_write(pin_data);
}

void send_set_DD_RAM_addr(struct pin_data *pin_data, uint8_t ram_addr)
{
    pin_data->RS = 0;
    pin_data->RW = 0;
    binary_to_D_array(0b10000000 | ram_addr, pin_data->D);

    lcd_write(pin_data);
}

void send_write_data(struct pin_data *pin_data, uint8_t data)
{
    pin_data->RS = 1;
    pin_data->RW = 0;
    binary_to_D_array(data, pin_data->D);

    lcd_write(pin_data);
}

// TODO how to elegantly change the d array to be safe size-wise?
void LCD_setup(uint8_t rs, uint8_t rw, uint8_t e, uint8_t d[LCD_DATA_PIN_COUNT])
{
    g_pin_mappings.RS = rs; pinMode(g_pin_mappings.RS, OUTPUT);
    g_pin_mappings.RW = rw; pinMode(g_pin_mappings.RS, OUTPUT);
    g_pin_mappings.E = e;   pinMode(g_pin_mappings.E, OUTPUT);

    pinMode(DBG_PIN, OUTPUT);

    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {

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
    send_init(&g_pin_data);
    delay(7);
    send_init(&g_pin_data);
    delay(2);
    send_init(&g_pin_data);

    /* second part of init diagram */
    send_function_set(&g_pin_data, true, true, false);
    send_display_on_off(&g_pin_data, false, false, false);
    send_clear_display(&g_pin_data);
    send_entry_mode_set(&g_pin_data, true);

    g_setup_state = INITIALIZED;
    return true;
}

void LCD_test_write(void)
{
    /* 2 lines */
    send_function_set(&g_pin_data, true, true, false);
    /* turn off display */
    send_display_on_off(&g_pin_data, false, false, false);
    /* display on */
    send_display_on_off(&g_pin_data, true, true, true);
    send_entry_mode_set(&g_pin_data, true);

    /* S */
    send_write_data(&g_pin_data, 0b01010011);

    /* DEC */
    send_write_data(&g_pin_data, 0b01000100);
    send_write_data(&g_pin_data, 0b01000101);
    send_write_data(&g_pin_data, 0b01000011);

    /* Second line */
    send_set_DD_RAM_addr(&g_pin_data, 0b1000000);

    /* CR */
    send_write_data(&g_pin_data, 0b01000011);
    send_write_data(&g_pin_data, 0b01010010);
}
