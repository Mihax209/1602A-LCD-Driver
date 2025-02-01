#include <Arduino.h>

#define D0  (5)
#define D1  (6)
#define D2  (7)
#define D3  (8)
#define D4  (9)
#define D5  (10)
#define D6  (11)
#define D7  (12)

#define RS  (2)
#define RW  (3)
#define E   (4)

#define RS_IDX  (9)
#define RW_IDX  (8)

#define DBG_PIN (13)

#define LCD_ARRAY_LEN   (10)

int lcd_pins[LCD_ARRAY_LEN] = { D0, D1, D2, D3, D4, D5, D6, D7, RW, RS };

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
    for (int i = 0; i < LCD_ARRAY_LEN; ++i)
    {
        digitalWrite(lcd_pins[i], LOW);
    }
}

void write_lcd(uint16_t num)
{
    uint8_t pin_value;

    DBG(HIGH);

    digitalWrite(RS, get_pin_value(num, RS_IDX));
    digitalWrite(RW, get_pin_value(num, RW_IDX));

    for (int i = 0; i < LCD_ARRAY_LEN - 2; ++i)
    {
        pin_value = get_pin_value(num, i);
        digitalWrite(lcd_pins[i], pin_value);
    }

    digitalWrite(E, HIGH);
    delayMicroseconds(1);
    digitalWrite(E, LOW);
    lcd_pins_off();
    delayMicroseconds(90);

    DBG(LOW);
}

void write_lcd_4_bit(uint8_t num)
{
    write_lcd(num << 4);
}

void general_setup(void)
{
    pinMode(DBG_PIN, OUTPUT);
}

void setup_lcd(void)
{
    for (int i = 0; i < LCD_ARRAY_LEN; ++i)
    {
        pinMode(lcd_pins[i], OUTPUT);
    }

    pinMode(E, OUTPUT);
}

void init_lcd(void)
{
    /* turn off display */
    write_lcd(0b00000001000);

    /* init diagram */
    delay(30);
    write_lcd(0b0000110000);
    delay(7);
    write_lcd(0b0000110000);
    delay(2);
    write_lcd(0b0000110000);

    /* second part of init diagram */
    write_lcd(0b0000111100);
    write_lcd(0b0000001000);
    write_lcd(0b0000000001); delay(2);
    write_lcd(0b0000000110);
}

void test_write_lcd()
{
    /* 2 lines */
    write_lcd(0b0000111000);
    /* turn off display */
    write_lcd(0b00000001000);
    /* display on */
    write_lcd(0b0000001110);
    write_lcd(0b0000000110);

    /* S */
    write_lcd(0b1001010011);

    /* DEC */
    write_lcd(0b1001000100);
    write_lcd(0b1001000101);
    write_lcd(0b1001000011);

    /* Second line */
    write_lcd(0b0011000000);

    /* CR */
    write_lcd(0b1001000011);
    write_lcd(0b1001010010);
}

void setup()
{
    general_setup();
    setup_lcd();
    init_lcd();
    delay(10);
    test_write_lcd();
    delay(10);
}

void loop()
{

}
