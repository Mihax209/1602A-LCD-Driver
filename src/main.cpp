#include <Arduino.h>
#include <Mihax_LCD1602.h>

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

void setup()
{
    uint8_t D_pin_mapping[] = { D7, D6, D5, D4, D3, D2, D1, D0 };

    LCD_setup(RS, RW, E, D_pin_mapping);
    LCD_init();
    LCD_test_write();
}

void loop()
{

}
