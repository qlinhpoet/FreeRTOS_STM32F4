#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void task1_handler(void* parameters);
static void task2_handler(void* parameters);

int main(void)
{

  TaskHandle_t task1_handle;
  TaskHandle_t task2_handle;

  BaseType_t status;

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  //Enable the CYCCNT counter.
  DWT_CTRL |= ( 1 << 0);

  SEGGER_SYSVIEW_Conf();

  SEGGER_SYSVIEW_Start();
  //task creation
  status = xTaskCreate(task1_handler, "Task-1", 200, "Hello world from Task-1\n", 2, &task1_handle);
  status = xTaskCreate(task2_handler, "Task-2", 200, "Hello world from Task-2\n", 2, &task2_handle);

  //start the freeRTOS scheduler
  vTaskStartScheduler();

  while (1)
  {
	  printf("hello\n");
  }
}

int __io_putchar(int ch)
{
 // Write character to ITM ch.0
 ITM_SendChar(ch);
 return(ch);
}

static void task1_handler(void* parameters)
{

	char msg[100];

	while(1)
	{
		printf((char*)parameters);
		taskYIELD();
	}

}


static void task2_handler(void* parameters)
{
	char msg[100];
	while(1)
	{
		printf((char*)parameters);
		taskYIELD();
	}

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

void Error_Handler(void)
{

}

