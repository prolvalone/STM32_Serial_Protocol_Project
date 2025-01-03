#include "mpu6050.h"

extern I2C_HandleTypeDef hi2c1;

void mpu6050_Init(){
	// config Gyro
	uint8_t config_bit = FS_GYRO;
	HAL_I2C_Mem_Write(&hi2c1, (MP6050_ADDRESS << 1)+0 , REG_GYRO_CONFIG, 1, &config_bit, 1, 1000);	
	// Config Accelerometer
	config_bit = FS_ACC;
	HAL_I2C_Mem_Write(&hi2c1, (MP6050_ADDRESS << 1)+0 , REG_ACC_CONFIG, 1, &config_bit, 1, 1000);	
	// turn on Temp Sensor
	config_bit = 0;
	HAL_I2C_Mem_Write(&hi2c1, (MP6050_ADDRESS << 1)+0 , REG_USR_CTRL, 1, &config_bit, 1, 1000);	
}

uint16_t mpu6050_mem_read(){
	//Acc
	uint8_t buffer[2];
	uint16_t x_acc;
	HAL_I2C_Mem_Read(&hi2c1, (MP6050_ADDRESS << 1)+0 , REG_DATA, 1, buffer, 2, 1000);	
	x_acc = ((int16_t)buffer[0] << 8) + buffer[1];
	return x_acc;
	//Gyro
	
	//temp
}