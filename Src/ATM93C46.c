#include "ATM93C46.h"

extern SPI_HandleTypeDef hspi1;

void Eeprom_enable(
	HAL_SPI_Transmit(&hspi1, EWEN, 1, 1000);
);
void Eeprom_write(uint8_t eeprom_address, uint8_t data){ // Write Instructions

	SPI_Buffer[0] = (1 << 7) | (0x01 << 5) | (eeprom_address >> 2); //101& &&&&
	SPI_Buffer[1] = (eeprom_address << 6) | (data >> 2);
	SPI_Buffer[2] = (data << 6);
	HAL_SPI_Transmit(&hspi1, SPI_Buffer, 3, 1000);
	
}; 
