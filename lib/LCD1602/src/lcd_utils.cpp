#include <Arduino.h>
#include "LCD1602_internal.h"

void DBG(uint8_t val)
{
    digitalWrite(LED_BUILTIN, val);
    digitalWrite(DBG_PIN, val);
}

void lcd_pins_off(struct pin_mapping *pin_mappings)
{
    digitalWrite(pin_mappings->RS, LOW);
    digitalWrite(pin_mappings->RW, LOW);

    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        digitalWrite(pin_mappings->D[i], LOW);
    }
}

void lcd_write(struct pin_mapping *pin_mappings, struct pin_data *pin_data)
{
    /**
     * NOTE since this function is always used with the g_pin_data global,
     * we technically don't need a pointer to a pin_data struct. However, I think
     * it's more elegant to have a pointer given anyway, since otherwise it would be
     * easier to potentially call this function with an uninitialized g_pin_data struct
     */
    DBG(HIGH);

    digitalWrite(pin_mappings->RS, pin_data->RS);
    digitalWrite(pin_mappings->RW, pin_data->RW);

    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        digitalWrite(pin_mappings->D[i], pin_data->D[i]);
    }

    digitalWrite(pin_mappings->E, HIGH);
    delayMicroseconds(1);
    digitalWrite(pin_mappings->E, LOW);
    lcd_pins_off(pin_mappings);
    delayMicroseconds(90);

    DBG(LOW);
}

void binary_to_D_array(uint8_t value, uint8_t *D_array)
{
    for (int i = 0; i < LCD_DATA_PIN_COUNT; ++i)
    {
        /* NOTE MSb is the last array entry, i.e. D7 == D[7] */
        uint8_t mask = (1 << i) & value;
        D_array[i] = (mask != 0) ? 1 : 0;
    }
}
