#include "main.h"

#ifndef ATM93C46_H
#define ATM93C46_H



void Eeprom_enable(); // EWEN instruction
void Eeprom_write(uint8_t eeprom_address, uint8_t data); // Write Instructions
void Eeprom_read(uint8_t eeprom_address); // read Instructions

#endif // ATM93C46_H