#include "main.h"

#ifndef ATM93C46_H
#define ATM93C46_H
void Atm93c46_Write_Bit(uint8_t bit);
void Atm93c46_Send_Command(uint16_t command, uint8_t bit_count);
void Atm93c46_EWEN();
void Atm93c46_EWDS();
void Atm93c46_WRAL(uint8_t data);
void Atm93c46_ERAL();
void Atm93c46_WRITE(uint8_t address, uint8_t data);
void Atm93c46_ERASE(uint8_t address);
uint8_t Atm93c46_READ(uint8_t address);
void Atm93c46_Send_Data(uint8_t data);
void delay_us(uint16_t us);

#endif // ATM93C46_H