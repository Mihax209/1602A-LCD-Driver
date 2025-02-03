#include <Arduino.h>
#include <LCD1602.h>

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
    uint8_t D_pin_mapping[] = { D0, D1, D2, D3, D4, D5, D6, D7 };

    Serial.begin(115200);

    LCD_setup(RS, RW, E, D_pin_mapping);
    LCD_init();
    delay(5);
    String s = "blyat blyat cyka blyat";
    LCD_write_string(s);
    // LCD_test_write();
}

void loop()
{

}
