#include "ATM93C46.h"

extern SPI_HandleTypeDef hspi1;
extern uint8_t SPI_Buffer[3];
extern uint8_t EEPROM_Data[2];


void Eeprom_enable(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);

	SPI_Buffer[0] = 0b10011000;
	SPI_Buffer[1] = 0b00000000;

	HAL_SPI_Transmit(&hspi1, SPI_Buffer, 2, 1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
};
void Eeprom_write(uint8_t eeprom_address, uint8_t data){ // Write Instructions
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // CS HIGH
	SPI_Buffer[0] = (1 << 7) | (0b01 << 5) | (eeprom_address >> 2); //101& &&&&
	SPI_Buffer[1] = (eeprom_address << 6) | (data >> 2);
	SPI_Buffer[2] = (data << 6);
	HAL_SPI_Transmit(&hspi1, SPI_Buffer, 3, 1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // CS low
}; 

void Eeprom_read(uint8_t eeprom_address){ // Write Instructions
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // cs high
	SPI_Buffer[0] = (1 << 7) | (0b10 << 5) | (eeprom_address >> 2); //110& &&&&
	SPI_Buffer[1] = (eeprom_address << 6);
	HAL_SPI_Transmit(&hspi1, SPI_Buffer, 2, 1000); // Send READ instruction
	HAL_SPI_Receive(&hspi1, EEPROM_Data, 2, 1000);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // cs low
}; 