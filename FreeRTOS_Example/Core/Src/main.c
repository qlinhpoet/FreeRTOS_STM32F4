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

static void LedGreenHandler(void* para);
static void LedOrangeHandler(void* para);
static void LedRedHandler(void* para);

int main(void)
{

  TaskHandle_t LedGreenHandle, LedOrangeHandle, LedRedHandle;

  BaseType_t status;

  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  //Enable the CYCCNT counter.
  DWT_CTRL |= ( 1 << 0);

  SEGGER_SYSVIEW_Conf();

  SEGGER_SYSVIEW_Start();

  //task creation
  status = xTaskCreate(LedGreenHandler, "GreedTask", 200, NULL, 2, &LedGreenHandle);
  status = xTaskCreate(LedOrangeHandler, "OrangeTask", 200, NULL, 2, &LedOrangeHandle);
  status = xTaskCreate(LedRedHandler, "RedTask", 200, NULL, 2, &LedRedHandle);
  if (status != pdPASS)
  {
	  printf("Task creation failed!\n");
	  while(1);  // Dừng nếu task không tạo được
  }

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

static void LedGreenHandler(void* para)
{
	while(1)
	{
		GPIOD->ODR ^= 1<<12;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

static void LedOrangeHandler(void* para)
{
	while(1)
	{
		GPIOD->ODR ^= 1<<13;
		vTaskDelay(pdMS_TO_TICKS(800));
	}
}

static void LedRedHandler(void* para)
{
	while(1)
	{
		GPIOD->ODR ^= 1<<14;
		vTaskDelay(pdMS_TO_TICKS(400));
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

  /* Led configuration */
  RCC->AHB1ENR |= 1<<3;	//gpio D clock enable
  GPIOD->MODER &= (uint32_t)~0xFF000000;
  GPIOD->MODER |= (uint32_t)0x55000000;		//output mode
}

void Error_Handler(void)
{

}

