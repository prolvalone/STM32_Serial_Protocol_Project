#include "main.h"

#ifndef ATM93C46_H
#define ATM93C46_H

#define EWEN 0x1001100000


uint8_t SPI_Buffer[3];

void Eeprom_enable(); // EWEN instruction
void Eeprom_write(uint8_t eeprom_address, uint8_t data); // Write Instructions


#endif // ATM93C46_H