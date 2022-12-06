#ifndef TRANSMIT 
#define TRANSMIT
#include "main.h"
#include "stm32f4xx_hal.h"
#define dataL 66
extern uint16_t data[];
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

	if(delay == 0)
			return;
	TIM1->SR = 0;
	TIM1->ARR = delay;
	TIM1->CNT = 0;
	TIM1->CR1 |= 1; //enables timer 1
	while(!(TIM1->SR & TIM_SR_UIF));
	TIM1->CR1 &= ~(1); //disables timer 1
}
void Transmit(){
	int on = 0;
	for(size_t i = 0; i<dataL;++i){
		Toggle(on);
		on^=1;
		Delay(data[i]);
	}
	HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);

}

#endif
