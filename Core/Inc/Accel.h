#ifndef ACCEL
#define ACCEL
#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"

#define ACC_WRITE 0b00110010
#define ACC_READ 0b00110011

#define CR1 0x20
#define CR1_VAL 0b00111111

#define CR4 0x23
#define CR4_VAL 0x08

#define X 0x28
extern I2C_HandleTypeDef hi2c3;
#define timeout 30
#define OFFthreshold 1300
#define ONthreshold 701

int Awake = 0;
uint8_t buff[10];
int16_t xyz_old[3];
int16_t xyz[3];
int cycles_still = 0;


void poll_accel(){
	 buff[0] =  (uint8_t)X;
	buff[0] |= 1 << 7;


	HAL_StatusTypeDef transmitStatus = HAL_I2C_Master_Transmit(&hi2c3, ACC_WRITE, &buff[0], 1, 1000);

	HAL_StatusTypeDef receiveStatus = HAL_I2C_Master_Receive(&hi2c3, ACC_READ, &buff[0], 6, 1000);

	int16_t x = ((int16_t) buff[1] << 8) | (buff[0]);
	int16_t y = ((int16_t) buff[3] << 8) | (buff[2]);
	int16_t z = ((int16_t) buff[5] << 8) | (buff[4]);
	 xyz[0] = x;
	 xyz[1] = y;
	 xyz[2] = z;
}

void accel_init(){
	 buff[0] = (uint8_t)CR1;
	  buff[1] = (uint8_t)CR1_VAL;
	  HAL_StatusTypeDef tStat = HAL_I2C_Master_Transmit(&hi2c3, ACC_WRITE, &buff[0], 2, 1000);
	  HAL_StatusTypeDef rStat = HAL_I2C_Master_Receive(&hi2c3, ACC_READ, &buff[0], 1, 1000);

	  buff[0] = (uint8_t)CR4;
	  	  buff[1] = (uint8_t)CR4_VAL;
	  	  HAL_I2C_Master_Transmit(&hi2c3, ACC_WRITE, &buff[0], 2, 1000);
	  	  HAL_I2C_Master_Receive(&hi2c3, ACC_READ, &buff[0], 1, 1000);
}
void AccelInit(){
    accel_init();
    poll_accel();
    poll_accel();
}
void resetCounter(){
    cycles_still = 0;
}

int poll(){
    if(!Awake){
	  		poll_accel();
	  		 if ((abs(xyz[2] - xyz_old[2]) > ONthreshold)){
	  			Awake = 1;
	  		 }
	  		 xyz_old[0] = xyz[0];
	  		 xyz_old[1] = xyz[1];
	  		 xyz_old[2] = xyz[2];
	  		 resetCounter();
	  	}
    else{
        poll_accel();
        if ((abs(xyz[2] - xyz_old[2]) < OFFthreshold)){
            cycles_still++;
                if(cycles_still > 20*timeout){
                    Awake = 0;
                    cycles_still = 0;
                }
        }
        else{
            cycles_still = 0;
        }
        xyz_old[0] = xyz[0];
        xyz_old[1] = xyz[1];
        xyz_old[2] = xyz[2];
    }
    return Awake;
}


#endif
