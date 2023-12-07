
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtos.c
  * @author  MCD Application Team
  * @brief   azure_rtos application implementation file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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

#include "string.h"
#include "inttypes.h"
#include "rtos.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

typedef TX_BYTE_POOL MemoryArea;
typedef TX_BLOCK_POOL MemoryBlocks;

#define CreateArea _tx_byte_pool_create
#define CreateBlocks _tx_block_pool_create
#define AllocateArea _tx_byte_allocate
#define AllocateBlocks _tx_block_pool_allocate
#define CreateActivity tx_thread_create
#define Activity 		TX_THREAD
#define ActionFlags TX_EVENT_FLAGS_GROUP
#define ActionQueue TX_QUEUE

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN TX_Pool_Buffer */
/* USER CODE END TX_Pool_Buffer */
static MemoryArea SystemMemory;
static MemoryBlocks SystemRegistry;

#define ACT_STACK_SIZE       		1024
#define SYS_STACK_SIZE    		4096
#define ACT_BLOCKS_SIZE    		64
#define SYS_BLOCKS_SIZE  	256
#define ACT_INBOX_SIZE          	64
#define SYS_INBOX_SIZE          	256

Activity                    SystemProcess;
ActionQueue                 SystemActions;
ActionQueue                 ComputativeActions;
ActionQueue                 OperativeActions;
ActionFlags                 ActionOrder;


/* USER CODE BEGIN PV */
typedef struct
{
	uint64_t Id;
	char      Name[128];
	uint32_t Address;
	size_t   Size;
	size_t   ItemSize;
	void *   Target;
	void*    SyncTool;
	char     Type[128];
	uint8_t		  HashCode[8];
	uint8_t		  SerialCode[24];
}RegistryEntry;

void    MainActivity(ULONG thread_input);


VOID tx_application_define(VOID *first_unused_memory)
{
  /* USER CODE BEGIN  tx_application_define */

  /* USER CODE END  tx_application_define */

  VOID *usagePointer;

  CreateArea(&SystemMemory, "SystemMemory", first_unused_memory, SYSTEM_MEMORY_SIZE);

  CreateBlocks(&SystemRegistry, "SystemRegistry", sizeof(RegistryEntry) , &SystemMemory, 255);

  AllocateArea(&SystemMemory, (VOID **) &usagePointer, SYS_STACK_SIZE, TX_NO_WAIT);

  CreateActivity(&SystemProcess, (char*)("mainactivity"), MainActivity, 0, usagePointer, SYS_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);

  usagePointer = (VOID *)&SystemMemory;

    if (App_ThreadX_Init(usagePointer) != TX_SUCCESS)
    {
      /* USER CODE BEGIN  App_ThreadX_Init_Error */

      /* USER CODE END  App_ThreadX_Init_Error */
    }

    /* USER CODE BEGIN  App_ThreadX_Init_Success */

    /* USER CODE END  App_ThreadX_Init_Success */

  }


/* USER CODE BEGIN  0 */

/* USER CODE END  0 */
