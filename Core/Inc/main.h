/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DISP_LCD_BL_Pin GPIO_PIN_0
#define DISP_LCD_BL_GPIO_Port GPIOH
#define XL_Pin GPIO_PIN_0
#define XL_GPIO_Port GPIOA
#define YU_Pin GPIO_PIN_1
#define YU_GPIO_Port GPIOA
#define XR_Pin GPIO_PIN_2
#define XR_GPIO_Port GPIOA
#define DISP_nCS_Pin GPIO_PIN_3
#define DISP_nCS_GPIO_Port GPIOA
#define YD_Pin GPIO_PIN_4
#define YD_GPIO_Port GPIOA
#define ENCODER_B_Pin GPIO_PIN_5
#define ENCODER_B_GPIO_Port GPIOA
#define ENCODER_B_EXTI_IRQn EXTI5_IRQn
#define ENCODER_A_Pin GPIO_PIN_6
#define ENCODER_A_GPIO_Port GPIOA
#define ENCODER_SW_Pin GPIO_PIN_7
#define ENCODER_SW_GPIO_Port GPIOA
#define ENCODER_SW_EXTI_IRQn EXTI7_IRQn
#define LED_RGB_DATA_Pin GPIO_PIN_0
#define LED_RGB_DATA_GPIO_Port GPIOB
#define DISP_TE_Pin GPIO_PIN_2
#define DISP_TE_GPIO_Port GPIOB
#define DISP_WR_Pin GPIO_PIN_10
#define DISP_WR_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_12
#define RS485_RX_GPIO_Port GPIOB
#define RS485_TX_Pin GPIO_PIN_13
#define RS485_TX_GPIO_Port GPIOB
#define DISP_RD_Pin GPIO_PIN_11
#define DISP_RD_GPIO_Port GPIOD
#define DISP_DCX_Pin GPIO_PIN_12
#define DISP_DCX_GPIO_Port GPIOD
#define DISP_nRESET_Pin GPIO_PIN_8
#define DISP_nRESET_GPIO_Port GPIOA
#define RS485_DIR_Pin GPIO_PIN_9
#define RS485_DIR_GPIO_Port GPIOA
#define IM2_Pin GPIO_PIN_4
#define IM2_GPIO_Port GPIOB
#define IM1_Pin GPIO_PIN_5
#define IM1_GPIO_Port GPIOB
#define IM0_Pin GPIO_PIN_6
#define IM0_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
