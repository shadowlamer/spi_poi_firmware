//
// Created by sl on 14.02.2020.
//

#ifndef SPIPOI_LED_H
#define SPIPOI_LED_H

#include "stm32f1xx_hal.h"
#include "spi.h"

#define PREAMBLE_SIZE 4
#define BYTES_PER_LED 4
#define NUM_LEDS 36
#define SPI_BUF_SIZE (NUM_LEDS * BYTES_PER_LED + PREAMBLE_SIZE)
#define LED_POWER_MAX 31

extern uint8_t *led_buf;

void led_init();
void set_led(uint8_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t power);
void black_all();
void init_buf();

#endif //SPIPOI_LED_H
