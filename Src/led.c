//
// Created by sl on 14.02.2020.
//

#include "led.h"

uint8_t spi_buf[SPI_BUF_SIZE];
uint8_t *led_buf = spi_buf + PREAMBLE_SIZE;

void led_init() {
  init_buf();
  HAL_SPI_Transmit_DMA(&hspi2,spi_buf,SPI_BUF_SIZE);
}

void set_led(uint8_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t power) {
  uint16_t addr = num * BYTES_PER_LED;
  led_buf[addr + 0] = 0xe0 | (power & 0x1f);
  led_buf[addr + 1] = b;
  led_buf[addr + 2] = g;
  led_buf[addr + 3] = r;
}

void black_all () {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    set_led(i, 0,0,0,0);
  }
}

void init_buf () {
  for (uint16_t i = 0; i < SPI_BUF_SIZE; i++) {
    spi_buf[i] = 0;
  }
  black_all();
}
