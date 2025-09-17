/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpu6050.h>
#include <ATM93C46.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SCK PA5 // SPI, PA5
#define MISO PA6 // SPI, Blue
#define MOSI PA7 // SPI Green
#define CS PA10 // SPI, white; uses GPIO
#define SCL PB8 // i2c gray
#define SDA PB9 // i2c purple
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t 	sensor_buffer[3] = {0}; 		// for recieving data from sensor						// for outputting to serial with PUTTY
char 	menu[] = "WELCOME TO EEPROM DEVICE MANAGER\r\n"
											"Enter Your Desired Action:\r\n"
											"1. ERASE\r\n"
											"2. ERAL\r\n"
											"3. WRITE\r\n"
											"4. WRAL\r\n"
											"5. READ\r\n"
											"6. EWDS\r\n";
uint8_t 	EEPROM_Data[2];

uint16_t result_x_acc;									//data from sensor

uint8_t ReadValue;
static int rx_indx; // index of the user input
uint8_t rx_data = 0;
uint8_t rx_buffer[100];
uint8_t transfer_complete;

typedef enum{
	CMD_NONE,
	CMD_ERASE,
	CMD_WRITE,
	CMD_READ,
	CMD_WRAL
	
}CmdState;
CmdState current_state = CMD_NONE;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void interpretCommand(uint8_t *cmd);
void uartSend(char *msg);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim1);  // start the Timer1
	HAL_Delay(500);
	mpu6050_Init();
	Atm93c46_EWEN(); // Read and write enable EEPROM
	
	HAL_UART_Transmit(&huart2, (uint8_t*)menu, strlen(menu), HAL_MAX_DELAY); // send welcome message and menu
	HAL_UART_Transmit(&huart2, (uint8_t*)"Enter Here: ", 12, HAL_MAX_DELAY); // send enter here
	HAL_UART_Receive_IT(&huart2, &rx_data, 1); // recieve input
	
	
	
	
	
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		
		
  
	/****************************************************************************************************************************/
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV4;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00402D41;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 11;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SHIFT_CLOCK_Pin|DATA_IN_Pin|CHIP_SELECT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SHIFT_CLOCK_Pin DATA_IN_Pin */
  GPIO_InitStruct.Pin = SHIFT_CLOCK_Pin|DATA_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DATA_OUT_Pin */
  GPIO_InitStruct.Pin = DATA_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DATA_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CHIP_SELECT_Pin */
  GPIO_InitStruct.Pin = CHIP_SELECT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CHIP_SELECT_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
  /* Prevent unused argument(s) compilation warning */
  

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */
		
		//recieve the sent char
		if (rx_data == '\r' || rx_data == '\n') {
        rx_buffer[rx_indx] = '\0';  // terminate
        rx_indx = 0;
        HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
				
				interpretCommand(rx_buffer);
    } else {
        rx_buffer[rx_indx++] = rx_data;
        HAL_UART_Transmit(&huart2, &rx_data, 1, HAL_MAX_DELAY);
    }
    HAL_UART_Receive_IT(&huart2, &rx_data, 1);
		
		
	
}

void interpretCommand(uint8_t *cmd)
{
    switch(current_state) {
        case CMD_NONE:
            switch(cmd[0]) {
                case '1':   // ERASE
                    uartSend("Please Enter the Memory Location To Erase: ");
                    current_state = CMD_ERASE;
                    break;

                case '2':   // ERAL
                    Atm93c46_ERAL();
                    uartSend("Successfully Erased All Memory Locations\r\n");
										uartSend("Enter Here: ");
                    break;

                case '3':   // WRITE
                    uartSend("Please Enter the Memory Location to Write: ");
                    current_state = CMD_WRITE;
                    break;

                case '4':   // WRAL
                    uartSend("Please Enter the Value to WRAL: ");
                    current_state = CMD_WRAL;
                    break;

                case '5':   // READ
                    uartSend("Please Enter the Memory Location to READ: ");
                    current_state = CMD_READ;
                    break;

                case '6':   // EWDS
                    Atm93c46_EWDS();
                    uartSend("Disabled Writing to Chip\r\n");
										uartSend("Enter Here: ");
                    break;

                default:
                    uartSend("Unknown command\r\n");
                    break;
            }
            break;   

        case CMD_ERASE:
        {
            uint8_t addr = (uint8_t)strtoul((char*)cmd, NULL, 16);
            Atm93c46_ERASE(addr);

            char msg[50];
            sprintf(msg, "Successfully Erased %u\r\n", (uint8_t)addr);
            uartSend(msg);

            current_state = CMD_NONE;
            uartSend("Enter Here: ");
        }
        break;

        case CMD_WRITE:
        {
            uint8_t addr = (uint8_t)strtoul((char*)cmd, NULL,16);
            Atm93c46_WRITE(addr, 0x00);

            char msg[50];
            sprintf(msg, "Successfully Wrote to %u\r\n", (uint8_t)addr);
            uartSend(msg);

            current_state = CMD_NONE;
            uartSend("Enter Here: ");
        }
        break;

        case CMD_WRAL:
        {
            uint8_t val = (uint8_t)strtoul((char*)cmd, NULL, 16);
            Atm93c46_WRAL(val);

            char msg[50];
            sprintf(msg, "Successfully Wrote Value %u to All\r\n", val);
            uartSend(msg);

            current_state = CMD_NONE;
            uartSend("Enter Here: ");
        }
        break;

        case CMD_READ:
        {
            uint8_t addr = (uint8_t)strtoul((char*)cmd, NULL, 16);
            uint8_t val  = Atm93c46_READ(addr);

            char msg[50];
            sprintf(msg, "Read Value %u from Address %u\r\n", val, (uint8_t)addr);
            uartSend(msg);

            current_state = CMD_NONE;
            uartSend("Enter Here: ");
        }
        break;
    }
}

void uartSend(char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
