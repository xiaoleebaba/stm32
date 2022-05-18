/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LENGTH 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// 以下注释代码为中断方式收发的代码
//uint8_t p_t_data[20] = "HelloWorld";
//uint8_t p_r_data[20] = {0};
//
//uint8_t tx_cplt_flag = 0;
//uint8_t rx_cplt_flag = 0;
//uint8_t p_buf[20] = {0};

// 以下为DMA方式，空闲中断收发
extern DMA_HandleTypeDef hdma_usart1_rx;
uint8_t RxBuffer[LENGTH];
uint8_t RecCount = 0;
uint8_t RxFlag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
    char *msg1 = "***** UART Communication using Idle IT + DMA *****\r\n";

    HAL_UART_Transmit(&huart1, msg1, strlen(msg1), 1000);
    // HAL_UART_Transmit(&huart1, msg2, strlen(msg2), 1000);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer, LENGTH);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//      HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//      HAL_Delay(500);
//      // printf("Hello, Usart\r\n");
//      HAL_UART_Receive(&huart1, p_data, 10, 1000);
//      //HAL_Delay(300);
//      printf("len: %d", strlen(p_data));
//      HAL_UART_Transmit(&huart1, p_data, 10, 1000);

//      if(rx_cplt_flag == 1)
//      {
//          rx_cplt_flag == 0;
//            printf("RXXX: \r\n");
//          HAL_UART_Transmit_IT(&huart1, p_r_data, 10);
//                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
//      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//      HAL_Delay(500);
//      }
//
//      if(tx_cplt_flag == 1)
//      {
//          printf("TXXX: \r\n");
//          tx_cplt_flag = 0;
//          HAL_UART_Receive_IT(&huart1, p_r_data, 10);
//      }

      //memset(p_data, 0, 20);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

      // HAL_UART_Transmit(&huart1, msg1, strlen(msg1), 1000);
      HAL_Delay(500);
      if(RxFlag == 1) {
          RxFlag = 0;
          RecCount = LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
          HAL_UART_Transmit_DMA(&huart1, (uint8_t *) RxBuffer, RecCount);
          RecCount = 0;
          __HAL_DMA_DISABLE(&hdma_usart1_rx);

          HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer, LENGTH);
          __HAL_DMA_ENABLE(&hdma_usart1_rx);
      }
  }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void com_handle(void)
{
    char *msg2 = "Please enter arbitrary length characters:\r\n";
    printf("%s", msg2);
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer, LENGTH);
    if(RxFlag == 1)
    {
        RxFlag = 0;
        RecCount = LENGTH - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) RxBuffer, RecCount);
        RecCount = 0;
        __HAL_DMA_DISABLE(&hdma_usart1_rx);

        HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer, LENGTH);
        __HAL_DMA_ENABLE(&hdma_usart1_rx);
    }
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
