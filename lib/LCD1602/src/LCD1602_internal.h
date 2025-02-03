#pragma once

#include <Arduino.h>

// TODO how to setup debug pin?
#define DBG_PIN (13)

#define LCD_DATA_PIN_COUNT  (8)

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
