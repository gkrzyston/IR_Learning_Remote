#ifndef TRANSMIT 
#define TRANSMIT
#include "main.h"
#include "stm32f4xx_hal.h"
extern uint16_t data[];
extern uint32_t dataLen;
extern TIM_HandleTypeDef htim3;

void Toggle(int on);
void Delay(uint16_t delay);
void GlobalTransmit();
void DeviceTransmit(uint16_t data[dataLen]);

#endif
