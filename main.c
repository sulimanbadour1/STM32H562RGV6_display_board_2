/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32h5xx_hal.h"
#include "stm32h5xx_it.h"

#include "global_variables.h"
#include "ili9488.h"
#include "GUI.h"
#include "modbus.h"
#include "ws2812.h"

#include <stdio.h>

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
ADC_HandleTypeDef hadc1;

DCACHE_HandleTypeDef hdcache1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
DMA_HandleTypeDef handle_GPDMA1_Channel2;

UART_HandleTypeDef huart5;
DMA_HandleTypeDef handle_GPDMA1_Channel0;

/* USER CODE BEGIN PV */

struct BMS_data_structure BMS_no1; // this structure holds all data read from BMS

// variables for rotary encoder
// Debounce delay times (in milliseconds)
#define AB_DEBOUNCE_DELAY_MS 50  // Short debounce for encoder A/B transitions
#define SW_DEBOUNCE_DELAY_MS 200 // Longer debounce for encoder push button

// Variables to track encoder position and debounce timing
volatile int16_t encoder_position = 0;
volatile uint8_t last_encoder_state = 0;
volatile uint32_t last_interrupt_time = 0; // Timestamp of last encoder A/B interrupt
volatile uint32_t last_sw_press_time = 0;  // Timestamp of last button press

// GUI variables
volatile uint8_t current_gui_screen = 1; // what GUI screen is now shown on LCD
volatile uint8_t new_gui_screen = 0;     // what GUI screen is now shown on LCD

// MODBUS variables

#ifdef LCD_IS_MODBUS_MASTER
uint8_t byte;
uint8_t response_buffer[UART_RX_BUFFER_SIZE];
volatile uint16_t response_index = 0;
volatile uint8_t modbus_receiving = 0;

// Dummy register storage
uint16_t modbus_registers[UART_RX_BUFFER_SIZE]; // Simulate up to 125 holding registers (max per Modbus spec)
#endif

// RGB led colors
uint32_t LED_colors[WS2812_LED_COUNT];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_GPDMA1_Init(void);
static void MX_UART5_Init(void);
static void MX_ICACHE_Init(void);
static void MX_ADC1_Init(void);
static void MX_DCACHE1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Set_BMS_dummy_values(void)
{
  // set some dummy variables
  BMS_no1.SOC = 501;                   // in 0.1 percent
  BMS_no1.Stack_Voltage_mV = 55123;    // in mV
  BMS_no1.current_userA = 22666;       // in 0.01A
  BMS_no1.Power_W = 288;               // in W
  BMS_no1.Internal_Temperature_C = 23; // in deg C

  BMS_no1.Cell_1_voltage_mV = 1;   // in mV
  BMS_no1.Cell_2_voltage_mV = 2;   // in mV
  BMS_no1.Cell_3_voltage_mV = 3;   // in mV
  BMS_no1.Cell_4_voltage_mV = 4;   // in mV
  BMS_no1.Cell_5_voltage_mV = 5;   // in mV
  BMS_no1.Cell_6_voltage_mV = 6;   // in mV
  BMS_no1.Cell_7_voltage_mV = 7;   // in mV
  BMS_no1.Cell_8_voltage_mV = 8;   // in mV
  BMS_no1.Cell_9_voltage_mV = 9;   // in mV
  BMS_no1.Cell_10_voltage_mV = 10; // in mV
  BMS_no1.Cell_11_voltage_mV = 11; // in mV
  BMS_no1.Cell_12_voltage_mV = 12; // in mV
  BMS_no1.Cell_13_voltage_mV = 13; // in mV
  BMS_no1.Cell_14_voltage_mV = 14; // in mV
  BMS_no1.Cell_15_voltage_mV = 15; // in mV
  BMS_no1.Cell_16_voltage_mV = 16; // in mV
}

// ONE second timer
/*
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim1);
}
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM1 calls every 1 second a periodic update
  // updates LCD etc.
  if (htim->Instance == TIM1)
  {
    if (current_gui_screen == 1)
    {
      if (new_gui_screen == 1)
      {
        ILI9488_FillScreen(BLACK); // redraw background color
        new_gui_screen = 0;
      }
      Plot_GUI_screen_2();
    }

    if (current_gui_screen == 2)
    {
      if (new_gui_screen == 1)
      {
        ILI9488_FillScreen(BLUE); // redraw background color
        new_gui_screen = 0;
      }
      Plot_GUI_screen_3();
    }

    // dummy update as demo
    // BMS_no1.SOC = BMS_no1.SOC + 1 ;

    // change RGB led color
    // to a random color
    // ws2812_send_random_color();	// 1 LED
    // ws2812_send_random_colors();	// 3 LEDs

    // ws2812_send_random_colors_with_brightness(50); // 3 LEDs with brightness control

    // Called every 1 second
    // WS2812_Send(LED_colors);
    // WS2812_send_random_colors();
    WS2812_send_random_colors_with_brightness(255); // LEDs with brightness control

#ifdef LCD_IS_MODBUS_MASTER
                                                    // LCD is MODBUS master
    Modbus_Request_ReadRegisters(0, 22); // Read first 22 registers from slave
#endif
  }

#ifdef LCD_IS_MODBUS_MASTER
  // LCD is MODBUS master
  // TIM6 handles UART receive IDLE line detection
  // after a MODBUS frame is received
  // when the line becomes idle after 100ms
  // the MODBUS packet is processed
  if (htim->Instance == TIM6 && modbus_receiving)
  {
    modbus_receiving = 0;
    HAL_TIM_Base_Stop_IT(&htim6);

    // Now the Modbus response is assumed complete

    Modbus_Response_Handler(response_buffer, response_index);
  }
#endif
}

// functions for rotary encoder

// Interrupt handler for encoder and button
// ENCODER SW resets encoder_position on FALLING EDGE
// ENCODER_A falling edge handler
/*
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
     uint32_t current_time = HAL_GetTick();

      // Encoder button (switch)
      if (GPIO_Pin == ENCODER_SW_Pin)
      {
          if (current_time - last_interrupt_time >= SW_DEBOUNCE_DELAY_MS)
          {
              last_interrupt_time = current_time;
              encoder_position = 0;
          }
          return;
      }

      // Handle A or B falling edge
      if (GPIO_Pin == ENCODER_A_Pin || GPIO_Pin == ENCODER_B_Pin)
      {
          if (current_time - last_interrupt_time >= AB_DEBOUNCE_DELAY_MS)
          {
              last_interrupt_time = current_time;

              uint8_t current_state = read_encoder_state();
              uint8_t combined = (last_encoder_state << 2) | current_state;

              switch (combined)
              {
                  case 0b0001:
                  case 0b0111:
                  case 0b1110:
                  case 0b1000:
                      encoder_position++;
                      break;

                  case 0b0010:
                  case 0b0100:
                  case 0b1101:
                  case 0b1011:
                      encoder_position--;
                      break;

                  default:
                      // Invalid transition (bouncing?) – ignore
                      break;
              }

              last_encoder_state = current_state;
          }
      }
}
*/

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == ENCODER_SW_Pin) // ENCODER SW pressed
  {
    uint32_t current_time = HAL_GetTick();
    if (current_time - last_interrupt_time >= SW_DEBOUNCE_DELAY_MS)
    {
      last_interrupt_time = current_time;
      encoder_position = 0; // zero encoder position

      // switch current gui screen
      current_gui_screen++;
      if (current_gui_screen > 2)
      {
        current_gui_screen = 1;
      }

      new_gui_screen = 1;
    }
  }

  if (GPIO_Pin == ENCODER_B_Pin) // ENCODER_B changed
  {
    uint32_t current_time = HAL_GetTick();

    // Debounce check
    if (current_time - last_interrupt_time >= AB_DEBOUNCE_DELAY_MS)
    {
      last_interrupt_time = current_time;

      GPIO_PinState a = HAL_GPIO_ReadPin(ENCODER_A_GPIO_Port, ENCODER_A_Pin);
      if (a == GPIO_PIN_SET)
      {
        encoder_position--; // counter clock wise
      }
      else
      {
        encoder_position++; // clock wise
      }
    }
  }
}

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
  MX_GPDMA1_Init();
  MX_UART5_Init();
  MX_ICACHE_Init();
  MX_ADC1_Init();
  MX_DCACHE1_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  // assign some RGB led colors for start
  LED_colors[0] = WS2812_RED;
  LED_colors[1] = WS2812_GREEN;
  LED_colors[2] = WS2812_BLUE;
  // LED_colors[WS2812_RED, WS2812_GREEN, WS2812_BLUE];
  WS2812_Send(LED_colors);

  // ws2812_init();					// initialize RGB led

  // TIM2->CNT = 0xFFFFFFF0;

  Set_BMS_dummy_values(); // set some dummy values to variable

  // uint32_t Timer_Clock_Frequency = HAL_RCC_GetPCLK1Freq();	// get what it the set timer frequency

  /*
    // set display mode to 8-bit mode MIPI-DBI
    HAL_GPIO_WritePin(IM0_GPIO_Port, IM0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IM1_GPIO_Port, IM1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IM2_GPIO_Port, IM2_Pin, GPIO_PIN_RESET);
  */

  // set display mode to 16-bit mode MIPI-DBI
  HAL_GPIO_WritePin(IM0_GPIO_Port, IM0_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(IM1_GPIO_Port, IM1_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(IM2_GPIO_Port, IM2_Pin, GPIO_PIN_RESET);
  HAL_Delay(10);

  // enable LCD back-light
  HAL_GPIO_WritePin(DISP_LCD_BL_GPIO_Port, DISP_LCD_BL_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  // initialize LCD
  ILI9488_Init();

  // display introduction screen
  Plot_GUI_screen_1();
  HAL_Delay(5000);

  // test GUI screen, display temperature and time
  Plot_Touchscreen_Temperature_Screen(16, 2025, 11, 25, 8, 32, 25);
  HAL_Delay(5000);

  Plot_Touchscreen_Schedule_Screen(2025, 11, 26, 19, 47, 33);
  HAL_Delay(5000);

  Plot_Touchscreen_AddSchedule_Screen(2025, 11, 26, 19, 47, 33);
  HAL_Delay(5000);

  // prepare for normal run with black LCd background
  ILI9488_FillScreen(BLACK);

  HAL_TIM_Base_Start_IT(&htim1); // start ONE second time
  HAL_Delay(10);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /*
      HAL_Delay(1000);
      ILI9488_FillScreen(GRAY);
      ILI9488_FillScreen(BLACK);
      ILI9488_FillScreen(BLUE);
      ILI9488_FillScreen(RED);
      ILI9488_FillScreen(GREEN);
      ILI9488_FillScreen(WHITE);
      ILI9488_FillScreen(MAGENTA);
      ILI9488_FillScreen(YELLOW);
      ILI9488_FillScreen(CYAN);
      ILI9488_FillScreen(GRAY);
*/

    /*
          HAL_Delay(1000);
          ILI9488_FilledRectangle(100, 100, 250, 200,RED);
          HAL_Delay(1000);
          ILI9488_DrawPixel(300, 300, YELLOW);
          HAL_Delay(1000);
          ILI9488_DrawChar(200, 200, 5, '4', GREEN, BLUE);
          HAL_Delay(1000);
          ILI9488_DrawString(100, 100, 3, "Test ABCD 0123456789", WHITE, MAGENTA);
          HAL_Delay(3000);
    */

    // Plot_GUI_screen_1();
    // HAL_Delay(5000);
    // Plot_GUI_screen_2();
    // HAL_Delay(5000);

    // display encoder
    // ILI9488_FilledRectangle(0, 350, 100, 420,BLACK); // overwrite background

    char buffer[50]; // buffer for strings
    sprintf(buffer, "%+06d", encoder_position);
    ILI9488_DrawString(350, 0, 3, buffer, WHITE, BLACK);
    HAL_Delay(10);
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

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_CSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_CSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 125;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
   */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
   */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
   */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */
}

/**
 * @brief DCACHE1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DCACHE1_Init(void)
{

  /* USER CODE BEGIN DCACHE1_Init 0 */

  /* USER CODE END DCACHE1_Init 0 */

  /* USER CODE BEGIN DCACHE1_Init 1 */

  /* USER CODE END DCACHE1_Init 1 */
  hdcache1.Instance = DCACHE1;
  hdcache1.Init.ReadBurstType = DCACHE_READ_BURST_WRAP;
  if (HAL_DCACHE_Init(&hdcache1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCACHE1_Init 2 */

  /* USER CODE END DCACHE1_Init 2 */
}

/**
 * @brief GPDMA1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */

  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
  HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
  HAL_NVIC_SetPriority(GPDMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(GPDMA1_Channel2_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  /* USER CODE BEGIN GPDMA1_Init 2 */

  /* USER CODE END GPDMA1_Init 2 */
}

/**
 * @brief ICACHE Initialization Function
 * @param None
 * @retval None
 */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache (default 2-ways set associative cache)
   */
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */
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
  // htim1.Init.Prescaler = 25000-1;  // 250MHz / 25000 = 10kHz
  // htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  // htim1.Init.Period = 10000-1;   // 10kHz / 10000 = 1Hz (1s)

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 25000 - 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 9999;
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
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 2;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);
}

/**
 * @brief TIM6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 25000 - 1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */
}

/**
 * @brief UART5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_9B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_EVEN;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart5, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart5, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISP_LCD_BL_GPIO_Port, DISP_LCD_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISP_nCS_Pin | RS485_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP_WR_Pin | IM1_Pin | IM0_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, DISP_RD_Pin | DISP_DCX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISP_nRESET_GPIO_Port, DISP_nRESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(IM2_GPIO_Port, IM2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DISP_LCD_BL_Pin */
  GPIO_InitStruct.Pin = DISP_LCD_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DISP_LCD_BL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3
                           PC4 PC5 PC6 PC7
                           PC8 PC9 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_nCS_Pin DISP_nRESET_Pin RS485_DIR_Pin */
  GPIO_InitStruct.Pin = DISP_nCS_Pin | DISP_nRESET_Pin | RS485_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ENCODER_B_Pin ENCODER_SW_Pin */
  GPIO_InitStruct.Pin = ENCODER_B_Pin | ENCODER_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ENCODER_A_Pin */
  GPIO_InitStruct.Pin = ENCODER_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ENCODER_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DISP_TE_Pin */
  GPIO_InitStruct.Pin = DISP_TE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DISP_TE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DISP_WR_Pin */
  GPIO_InitStruct.Pin = DISP_WR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(DISP_WR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_RD_Pin DISP_DCX_Pin */
  GPIO_InitStruct.Pin = DISP_RD_Pin | DISP_DCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : IM2_Pin */
  GPIO_InitStruct.Pin = IM2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IM2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : IM1_Pin IM0_Pin */
  GPIO_InitStruct.Pin = IM1_Pin | IM0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI5_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI5_IRQn);

  HAL_NVIC_SetPriority(EXTI7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI7_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
