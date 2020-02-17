#include "sens.h"

 
uint16_t sens_pin[SENS_NUM] = {
	BUTTON_Pin
};

GPIO_TypeDef *sens_port[SENS_NUM] = {
	BUTTON_GPIO_Port
};

uint8_t sens_default[SENS_NUM] = {
	GPIO_PIN_SET
};

uint8_t sens_state[SENS_NUM];
uint8_t sens_filter[SENS_NUM];
uint8_t sens_edge[SENS_NUM];
uint32_t sens_time[SENS_NUM];

void sens_init() {
	uint8_t i;
	for (i=0;i<SENS_NUM;i++) {
		sens_filter[i] = 0;
		sens_state[i] = 0;
	}
}

void sens_process() {
	uint8_t i;
	for (i=0;i<SENS_NUM;i++) {
		if (sens_default[i]!=HAL_GPIO_ReadPin(sens_port[i],sens_pin[i])) { //on
			if (sens_filter[i]<SENS_FILTER)
				sens_filter[i]++;
			else {
				if (GPIO_PIN_SET!=sens_state[i])
					sens_edge[i] = GPIO_PIN_SET;
				sens_state[i] = GPIO_PIN_SET;
			}
		} else {                                                          //off
      sens_time[i] = HAL_GetTick();
			if (sens_filter[i]>0)
				sens_filter[i]--;
			else {
				sens_edge[i] = GPIO_PIN_RESET;
				sens_state[i] = GPIO_PIN_RESET;		
			}
		}
	}	
}

uint8_t sens(uint8_t s) {
  if (s>=SENS_NUM)
    return GPIO_PIN_RESET;
  return sens_state[s];
}

uint8_t long_press(uint8_t s) {
  return ((HAL_GetTick() - sens_time[s]) > SENS_LONG_PRESS);
}

uint8_t btn_long(uint8_t s) {
  return long_press(s) && btn(s);
}

uint8_t btn(uint8_t s) {
	uint8_t res;
	if (s>=SENS_NUM)
		return GPIO_PIN_RESET;
	res = sens_edge[s];
	sens_edge[s] = GPIO_PIN_RESET;
	return res;
}	

