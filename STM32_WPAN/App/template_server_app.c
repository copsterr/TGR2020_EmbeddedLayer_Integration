/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : App/template_server_app.c
 * Description        : P2P Server Application
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN UserCode */
/* Includes ------------------------------------------------------------------*/
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "template_server_app.h"
#include "stm32_seq.h"

#include "main.h"

extern motion_state_t state;
extern motion_state_t old_state;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint16_t TimeStamp;
  uint16_t Temperature;
} TEMPLATE_ParameterCharValue_t;

typedef struct
{
  uint8_t  NotificationStatus;
  TEMPLATE_ParameterCharValue_t Parameter;
  int16_t UpdateParameterStep;
  uint8_t UpdateParameter_timer_Id;
} TEMPLATE_Server_App_Context_t;

/* Private defines -----------------------------------------------------------*/
#define PARAMETER_CHANGE_STEP            10
#define PARAMETER_UPDATE_PERIOD          (100*1000/CFG_TS_TICK_VAL) /*100ms*/
#define PARAMETER_VALUE_MAX_THRESHOLD    350
#define PARAMETER_VALUE_MIN_THRESHOLD    100

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
PLACE_IN_SECTION("BLE_APP_CONTEXT") static TEMPLATE_Server_App_Context_t TEMPLATE_Server_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TEMPLATE_UpdateParameter_Timer_Callback(void);
/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void TEMPLATE_APP_context_Init(void);
static void TEMPLATE_Send_Notification_Task(void);

static void TEMPLATE_UpdateParameter_Timer_Callback(void)
{
// UTIL_SEQ_SetTask( 1<<CFG_IdleTask_Update_Temperature, CFG_SCH_PRIO_0);
}

/* Public functions ----------------------------------------------------------*/
void TEMPLATE_STM_App_Notification(TEMPLATE_STM_App_Notification_evt_t *pNotification)
{
  switch(pNotification->Template_Evt_Opcode)
  {
    case TEMPLATE_STM_NOTIFY_ENABLED_EVT:
      TEMPLATE_Server_App_Context.NotificationStatus = 1;
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFICATION ENABLED\n");
      APP_DBG_MSG(" \n\r");
#endif

/* Start the timer used to update the characteristic */
// HW_TS_Start(TEMPLATE_Server_App_Context.UpdateParameter_timer_Id, PARAMETER_UPDATE_PERIOD);

      break; /* TEMPLATE_STM_NOTIFY_ENABLED_EVT */

    case TEMPLATE_STM_NOTIFY_DISABLED_EVT:
      TEMPLATE_Server_App_Context.NotificationStatus = 0;
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFICATION DISABLED\n");
      APP_DBG_MSG(" \n\r");
#endif
      /* Stop the timer used to update the characteristic */
      // HW_TS_Stop(TEMPLATE_Server_App_Context.UpdateParameter_timer_Id);
      break; /* TEMPLATE_STM_NOTIFY_DISABLED_EVT */
      
    case TEMPLATE_STM_WRITE_EVT:
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : WRITE EVENT RECEIVED\n");
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : 0x%x\n",pNotification->DataTransfered.pPayload[0]);
      APP_DBG_MSG(" \n\r");
#endif
      if(pNotification->DataTransfered.pPayload[0] == 0x00)
      {
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : START TASK 2 \n");
#endif
      }
      break; /* TEMPLATE_STM_WRITE_EVT */

#if(BLE_CFG_OTA_REBOOT_CHAR != 0)       
    case TEMPLATE_STM_BOOT_REQUEST_EVT:
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : BOOT REQUESTED\n");
    APP_DBG_MSG(" \n\r");
#endif

      *(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
      NVIC_SystemReset();

      break; /* TEMPLATE_STM_BOOT_REQUEST_EVT */
#endif
      
    default:
      break; /* DEFAULT */
  }

  return;
}

void TEMPLATE_APP_Init(void)
{
	/* Register task used to update the characteristic (send the notification) */
	  UTIL_SEQ_RegTask(1<<CFG_IdleTask_Update_Temperature, UTIL_SEQ_RFU, TEMPLATE_Send_Notification_Task);
  /* Create timer to handle the periodic proprietary temperature sensor data update. */
  HW_TS_Create(CFG_TIM_PROC_ID_ISR,
    &(TEMPLATE_Server_App_Context.UpdateParameter_timer_Id),
    hw_ts_Repeated,
    TEMPLATE_UpdateParameter_Timer_Callback);

  /**
   * Initialize Template application context
   */
  TEMPLATE_Server_App_Context.NotificationStatus=0;
  TEMPLATE_APP_context_Init();
  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void TEMPLATE_APP_context_Init(void)
{
  TEMPLATE_Server_App_Context.Parameter.TimeStamp = 0;
	TEMPLATE_Server_App_Context.Parameter.Temperature = 0;
	TEMPLATE_Server_App_Context.UpdateParameterStep = PARAMETER_CHANGE_STEP;
}

static void TEMPLATE_Send_Notification_Task(void)
{
  uint8_t value[4] = {0};

  value[0] = (uint8_t) (0x00);
  value[1] = (uint8_t) (0x00);
  // value[0] = (uint8_t)(TEMPLATE_Server_App_Context.Parameter.TimeStamp & 0x00FF);
  // value[1] = (uint8_t)(TEMPLATE_Server_App_Context.Parameter.TimeStamp >> 8);
  // value[2] = (uint8_t)(TEMPLATE_Server_App_Context.Parameter.Temperature & 0x00FF);
  // value[3] = (uint8_t)(TEMPLATE_Server_App_Context.Parameter.Temperature >> 8);

  switch (state)
  {
  case UNCONCIOUS:
    value[2] = (uint8_t) (0x00);
    value[3] = (uint8_t) (0x08);
    //HAL_UART_Transmit(&huart1, (uint8_t*) "Send DEAD\r\n", 11, 100);
    break;
  
  case FALL:
    value[2] = (uint8_t) (0x00);
    value[3] = (uint8_t) (0x80);
    //HAL_UART_Transmit(&huart1, (uint8_t*) "Send FALL\r\n", 11, 100);
    break;

  default:
    break;
  }

  old_state = state;
  // TEMPLATE_Server_App_Context.Parameter.Temperature += TEMPLATE_Server_App_Context.UpdateParameterStep;
  // TEMPLATE_Server_App_Context.Parameter.TimeStamp += PARAMETER_CHANGE_STEP;
  // if (TEMPLATE_Server_App_Context.Parameter.Temperature > PARAMETER_VALUE_MAX_THRESHOLD) {
  //   TEMPLATE_Server_App_Context.UpdateParameterStep = -PARAMETER_CHANGE_STEP;
  // }
  // else if (TEMPLATE_Server_App_Context.Parameter.Temperature < PARAMETER_VALUE_MIN_THRESHOLD)
  // {
  //   TEMPLATE_Server_App_Context.UpdateParameterStep = +PARAMETER_CHANGE_STEP;
  // }

  if(TEMPLATE_Server_App_Context.NotificationStatus)
  {
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFY CLIENT WITH NEW PARAMETER VALUE \n ");
    APP_DBG_MSG(" \n\r");
#endif
    TEMPLATE_STM_App_Update_Char(0x0000,(uint8_t *)&value);
  }
  else
  {
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : CAN'T INFORM CLIENT - NOTIFICATION DISABLED\n ");
#endif
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/* USER CODE END UserCode */
