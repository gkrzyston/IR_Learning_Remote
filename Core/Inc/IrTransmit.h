#ifndef TRANSMIT 
#define TRANSMIT
#include "main.h"
#include "stm32f4xx_hal.h"
extern uint16_t data[];
extern uint32_t dataLen;
extern TIM_HandleTypeDef htim3;
void Toggle(int on){
	if(!on){
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	}
	else{
		HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
	}
}
void Delay(uint16_t delay){
	/*Adapted from:
	https://hackaday.com/2020/12/11/bare-metal-stm32-blinky-and-the-secret-of-delay-functions/#:~:text=On%20an%20STM32%20MCU%2C%20we,also%20have%20to%20use%20interrupts
	Example 4
	*/
	if(delay == 0)
			return;
	TIM1->SR = 0;
	TIM1->ARR = delay;
	TIM1->CNT = 0;
	TIM1->CR1 |= 1; //enables timer 1
	while(!(TIM1->SR & TIM_SR_UIF));
	TIM1->CR1 &= ~(1); //disables timer 1
}
void GlobalTransmit(){
	int on = 0;
	for(size_t i = 0; i<dataLen;++i){
		Toggle(on);
		on^=1;
		Delay(data[i]);
	}
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
}
void DeviceTransmit(uint16_t data[dataLen]){
	int on =0;
	for(uint16_t *ptr = data; ptr<data+dataLen;++ptr){
		Toggle(on);
		on^=1;
		Delay(*ptr);
	}
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);
}

#endif
