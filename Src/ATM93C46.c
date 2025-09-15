#include "ATM93C46.h"

extern TIM_HandleTypeDef htim1;

void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
}

void Atm93c46_Send_Command(uint16_t command, uint8_t bit_count) {
    for (int i = bit_count - 1; i >= 0; i--) {
        uint8_t bit = (command >> i) & 0x01;
        Atm93c46_Write_Bit(bit);
        delay_us(10); // Increased delay for stability
    }
		HAL_GPIO_WritePin(GPIOA, DATA_IN_Pin, GPIO_PIN_RESET); // set DI back to low
}

void Atm93c46_Write_Bit(uint8_t bit) {
    HAL_GPIO_WritePin(GPIOA, DATA_IN_Pin, bit ? GPIO_PIN_SET : GPIO_PIN_RESET);
    delay_us(10);
    HAL_GPIO_WritePin(GPIOA, SHIFT_CLOCK_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(GPIOA, SHIFT_CLOCK_Pin, GPIO_PIN_RESET);
    delay_us(10);
}

void Atm93c46_Send_Data(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        uint8_t bit = (data >> i) & 0x01;
        Atm93c46_Write_Bit(bit);
    }
		HAL_GPIO_WritePin(GPIOA, DATA_IN_Pin, GPIO_PIN_RESET); // set DI back to low
}

void Atm93c46_EWEN(void) {
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);
    Atm93c46_Send_Command(0b1001111110, 10);
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
	
    delay_us(10);
}

void Atm93c46_EWDS(void) {
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);
    Atm93c46_Send_Command(0b1000011111, 10);
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
    delay_us(10);
}

void Atm93c46_ERAL(void) {
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);
    Atm93c46_Send_Command(0b1001011111, 10);
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
    HAL_Delay(5); 
}

uint8_t Atm93c46_READ(uint8_t address) {
    uint8_t data = 0;
    uint16_t command = (1 << 9) | (0b10 << 7) | (address & 0x7F);

    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);

    Atm93c46_Send_Command(command, 10);

    // Read 8 bits
    for (int i = 7; i >= 0; i--) {
        HAL_GPIO_WritePin(GPIOA, SHIFT_CLOCK_Pin, GPIO_PIN_SET);
        delay_us(10);

        if (HAL_GPIO_ReadPin(GPIOA, DATA_OUT_Pin) == GPIO_PIN_SET) {
            data |= (1 << i);
        }

        HAL_GPIO_WritePin(GPIOA, SHIFT_CLOCK_Pin, GPIO_PIN_RESET);
        delay_us(10);
    }

    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
    delay_us(10);
    
    return data;
}

void Atm93c46_WRITE(uint8_t address, uint8_t data) {
    uint16_t command = (1 << 9) | (0b01 << 7) | (address & 0x7F);

    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);

    Atm93c46_Send_Command(command, 10);
    Atm93c46_Send_Data(data);
    
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
    delay_us(10);

    // Busy-wait for the write cycle to complete
    // The DATA_OUT pin will go high when the write is finished
    while (HAL_GPIO_ReadPin(GPIOA, DATA_OUT_Pin) == GPIO_PIN_RESET) {
        delay_us(100); // Check less frequently
    }
}

void Atm93c46_WRAL(uint8_t data) {
    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);

    Atm93c46_Send_Command(0b1000111111, 10);
    Atm93c46_Send_Data(data);

    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
    HAL_Delay(5); 
}

void Atm93c46_ERASE(uint8_t address) {
    uint16_t command = (1 << 9) | (0b11 << 7) | (address & 0x7F);

    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_SET);
    delay_us(10);

    Atm93c46_Send_Command(command, 10);

    HAL_GPIO_WritePin(GPIOA, CHIP_SELECT_Pin, GPIO_PIN_RESET);
    HAL_Delay(5); // Long erase time
}