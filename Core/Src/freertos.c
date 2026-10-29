/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "drivers/ds18b20.h"
#include "drivers/ina219.h"
#include "drivers/mpu6050.h"
#include "drivers/st7789.h"
#include "drivers/fonts.h"
#include "drivers/can.h"
#include "drivers/modbus.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for sensortask */
osThreadId_t sensortaskHandle;
const osThreadAttr_t sensortask_attributes = {
  .name = "sensortask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for filtertask */
osThreadId_t filtertaskHandle;
const osThreadAttr_t filtertask_attributes = {
  .name = "filtertask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for displaytask */
osThreadId_t displaytaskHandle;
const osThreadAttr_t displaytask_attributes = {
  .name = "displaytask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for commutask */
osThreadId_t commutaskHandle;
const osThreadAttr_t commutask_attributes = {
  .name = "commutask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for sensorqueue */
osMessageQueueId_t sensorqueueHandle;
const osMessageQueueAttr_t sensorqueue_attributes = {
  .name = "sensorqueue"
};
/* Definitions for displayqueue */
osMessageQueueId_t displayqueueHandle;
const osMessageQueueAttr_t displayqueue_attributes = {
  .name = "displayqueue"
};
/* Definitions for commuqueue */
osMessageQueueId_t commuqueueHandle;
const osMessageQueueAttr_t commuqueue_attributes = {
  .name = "commuqueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void startsensortask(void *argument);
extern void startfiltertask(void *argument);
extern void startdisplaytask(void *argument);
extern void startcommutask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of sensorqueue */
  sensorqueueHandle = osMessageQueueNew (5, sizeof(sensordata_t*), &sensorqueue_attributes);

  /* creation of displayqueue */
  displayqueueHandle = osMessageQueueNew (5, sizeof(sensordata_t*), &displayqueue_attributes);

  /* creation of commuqueue */
  commuqueueHandle = osMessageQueueNew (5, sizeof(sensordata_t*), &commuqueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of sensortask */
  sensortaskHandle = osThreadNew(startsensortask, NULL, &sensortask_attributes);

  /* creation of filtertask */
  filtertaskHandle = osThreadNew(startfiltertask, NULL, &filtertask_attributes);

  /* creation of displaytask */
  displaytaskHandle = osThreadNew(startdisplaytask, NULL, &displaytask_attributes);

  /* creation of commutask */
  commutaskHandle = osThreadNew(startcommutask, NULL, &commutask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_startsensortask */
/**
  * @brief  Function implementing the sensortask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_startsensortask */
__weak void startsensortask(void *argument)
{
  /* USER CODE BEGIN startsensortask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END startsensortask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

