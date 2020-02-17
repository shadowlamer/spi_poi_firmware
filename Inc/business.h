//
// Created by sl on 14.02.2020.
//

#ifndef SPIPOI_BUSINESS_H
#define SPIPOI_BUSINESS_H

#include "buffer.h"
#include "led.h"
#include "sens.h"
#include "usb_device.h"

#define POWER_ON_TIMEOUT 1000
#define WAIT_FOR_USB 500
#define IMAGE_SIZE (NUM_LEDS * 1024 )

typedef enum {
    ST_OFF = 0,
    ST_IDLE,
    ST_WORK,
    ST_POWER_ON,
    ST_POWER_OFF
} state_t;

void business_init();
void business_process();

#endif //SPIPOI_BUSINESS_H

