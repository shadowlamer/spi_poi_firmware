/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sens_H
#define __sens_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"

#define SENS_FILTER 5
#define SENS_LONG_PRESS 3000

enum {
	BUTTON=0,
	SENS_NUM
};
	 
void sens_init(void);
void sens_process(void);
uint8_t sens(uint8_t sens);
uint8_t btn(uint8_t sens);
uint8_t long_press(uint8_t s);
uint8_t btn_long(uint8_t s);

#ifdef __cplusplus
}
#endif
#endif /*__ acc_H */

/**
  * @}
  */

/**
  * @}
  */
