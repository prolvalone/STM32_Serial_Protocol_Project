#include "main.h"

#ifndef MPU6050_H
#define MPU6050_H



#define MP6050_ADDRESS 	0x68

#define REG_DATA 				59
#define REG_GYRO_CONFIG 27
#define REG_ACC_CONFIG 	28
#define REG_USR_CTRL 		107

#define FS_GYRO					8
#define FS_ACC					8

void mpu6050_Init();
uint16_t mpu6050_mem_read();


#endif // MPU6050_H