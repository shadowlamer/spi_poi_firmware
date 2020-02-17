//
// Created by sl on 14.02.2020.
//

#include "business.h"

state_t state;
extern volatile uint32_t timer;

uint32_t pointer = 0;
uint16_t image = 0;
uint32_t image_start = 0;

void go(state_t new_state, uint32_t timeout) {

  switch (state) {
    case ST_IDLE:
      HAL_GPIO_WritePin(USB_DETECT_GPIO_Port, USB_DETECT_Pin, GPIO_PIN_RESET);
      break;
    default:
      break;
  }

  switch (new_state) {
    case ST_WORK:
      SD_init();
      image = 0;
      pointer = 0;
      break;
    case ST_IDLE:
      SD_init();
      HAL_GPIO_WritePin(USB_DETECT_GPIO_Port, USB_DETECT_Pin, GPIO_PIN_SET);
      break;
    default:
      break;
  }

  state = new_state;
  timer = timeout;
}

uint8_t expired() {
  return timer == 0;
}

void business_init() {
  led_init();
  sens_init();
  go(ST_OFF, 0);
}

void show_power_animation() {
  uint8_t led_num = (timer * NUM_LEDS) / POWER_ON_TIMEOUT;
  if (ST_POWER_ON == state) led_num = NUM_LEDS - led_num;
  black_all();
  set_led(led_num, 0,0,led_num, LED_POWER_MAX);
}

void show_idle_animation() {
  black_all();
  set_led(0, 0, 20, 0, LED_POWER_MAX);
}

void show_work_animation() {
  black_all();
  set_led(0, 1,1,1, 10);
}

uint8_t usb_enabled(){
  return (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED);
}

void show_next_image() {
  image ++;
  pointer = 0;
  image_start = ((image - 1) * IMAGE_SIZE * BYTES_PER_LED) / SD_SECTOR_SIZE;
}

void process_image() {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
          uint32_t data = get_item(image_start, pointer, BYTES_PER_LED);
          uint8_t r = data & 0xff;
          data >>= 8;
          uint8_t g = data & 0xff;
          data >>= 8;
          uint8_t b = data & 0xff;
          set_led(i, r,g,b, LED_POWER_MAX);
          pointer++;
          if (pointer >= IMAGE_SIZE) {
            pointer = 0;
          }
        }
}

void business_process() {
  sens_process();
  switch (state) {
    case ST_OFF:
      if (btn_long(BUTTON)) go(ST_POWER_ON, POWER_ON_TIMEOUT);
      break;
    case ST_IDLE:
      show_idle_animation();
      if (btn_long(BUTTON)) go(ST_POWER_OFF, POWER_ON_TIMEOUT);
      if (expired() && !usb_enabled()) go (ST_WORK, 0);
      break;
    case ST_WORK:
      if (image == 0) {
        show_work_animation();
      } else {
        process_image();
      }
      if (btn(BUTTON)) show_next_image();
      if (long_press(BUTTON)) go(ST_POWER_OFF, POWER_ON_TIMEOUT);
      break;
    case ST_POWER_ON:
      show_power_animation();
      if (expired()) go(ST_IDLE, WAIT_FOR_USB);
      break;
    case ST_POWER_OFF:
      show_power_animation();
      if (expired()) go(ST_OFF, 0);
      break;
    default:
      break;
  }
}

