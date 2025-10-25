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
#include "adc.h"
#include "gpio.h"

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

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 参考Project1原始设计的电池参数（14串锂电池：42V-58.8V）
#define BATTERY_VOLTAGE_MIN 42.0f   // 14串最低电压 (3.0V * 14)
#define BATTERY_VOLTAGE_MAX 58.8f   // 14串最高电压 (4.2V * 14)
#define ADC_VREF 3.3f               // ADC参考电压

// ADC采样值转电压的计算公式：voltage = (ADC_Value / 4095.0) * 58.0
// 这意味着：实际电压 = (ADC / 4095) * ADC_VREF * 分压比
// 所以：分压比 = 58.0 / 3.3 = 17.58
#define VOLTAGE_DIVIDER_RATIO 17.58f  // 从Project1原始设计计算得出

// 电量分级阈值（参考Project1原始设计）
#define VOLTAGE_LEVEL_1 50.4f  // LED0与LED0+LED1的分界点
#define VOLTAGE_LEVEL_2 53.9f  // LED0+LED1与LED0+LED1+LED2的分界点  
#define VOLTAGE_LEVEL_3 56.0f  // LED0+LED1+LED2与全部LED的分界点

// 按键相关变量
uint8_t openfilg = 1;  // 开关机标志 (默认开机)
uint32_t button_press_start_time = 0;  // 按键按下开始时间
uint8_t button_was_pressed = 0;  // 上一次循环按键状态
uint8_t key_display_active = 0;  // KEY按键按下时显示电量
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// 根据电压显示电量LED（参考Project1原始设计）
void display_battery_voltage(float voltage);
// 处理KEY按键（PA7）长按开关机
void handle_key_button(void);
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
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
  int Value = 0;
  float voltage = 0.0;
  uint32_t current_time = 0;
  
  // 初始化：HIN开机（参考Project1，初始为RESET）
  HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_RESET);
  
  // 初始化所有LED为关闭
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // 读取ADC电压（参考Project1原始设计）
	  HAL_ADC_Start(&hadc);
	  HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
	  Value = HAL_ADC_GetValue(&hadc);
	  voltage = (Value / 4095.0f) * 58.0f;  // 直接使用Project1的公式
	  
	  // 处理KEY按键（PA7）- 长按3秒开关机（无论开关机状态都要检测）
	  handle_key_button();
	  
	  // 根据开关机状态控制LED和电源（参考Project1原始设计）
	  if(openfilg == 1){
		  // ========== 开机状态 ==========
		  
		  // 按KEY显示电量（display_battery_voltage内部会控制HIN）
		  if(key_display_active == 1){
			  display_battery_voltage(voltage);  // 内部会根据电压设置HIN
		  } else {
			  // 不按KEY时：LED全部熄灭，但HIN仍需根据电压控制
			  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			  
			  // 即使不按KEY，也要控制HIN（根据电压保护）
			  if(voltage >= BATTERY_VOLTAGE_MIN && voltage <= BATTERY_VOLTAGE_MAX){
				  HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_RESET);  // 正常电压：电源通
			  } else {
				  HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_SET);  // 异常电压：电源断
			  }
		  }
		  
	  } else {
		  // ========== 关机状态 ==========
		  
		  // 所有LED熄灭
		  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		  
		  // HIN=SET（电源断）
		  HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_SET);
	  }
	  
	  HAL_Delay(50);  // 延时50ms
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief 根据电压显示电量LED（参考Project1原始设计）
 * @param voltage 当前电压值
 */
void display_battery_voltage(float voltage) {
    // 参考Project1的电压分级逻辑（每个分支都要设置HIN）
    if(voltage >= BATTERY_VOLTAGE_MIN && voltage < VOLTAGE_LEVEL_1){
        // 42-50.4V：LED0
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_RESET);  // 正常电压：电源通
    } else if(voltage >= VOLTAGE_LEVEL_1 && voltage < VOLTAGE_LEVEL_2){
        // 50.4-53.9V：LED0+LED1
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_RESET);  // 正常电压：电源通
    } else if(voltage >= VOLTAGE_LEVEL_2 && voltage < VOLTAGE_LEVEL_3){
        // 53.9-56.0V：LED0+LED1+LED2
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_RESET);  // 正常电压：电源通
    } else if(voltage >= VOLTAGE_LEVEL_3 && voltage <= BATTERY_VOLTAGE_MAX){
        // 56.0-58.8V：所有LED
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_RESET);  // 正常电压：电源通
    } else {
        // 异常电压（<42V 或 >58.8V）：LED0闪烁警告（1s周期）
        static uint32_t last_flash = 0;
        static uint8_t flash_state = 0;
        uint32_t now = HAL_GetTick();
        if(now - last_flash > 1000){  // 1秒闪烁一次
            flash_state = !flash_state;
            last_flash = now;
        }
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, flash_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(HIN_GPIO_Port, HIN_Pin, GPIO_PIN_SET);  // 异常电压：电源断（保护）
    }
}

/**
 * @brief 处理KEY按键（PA7）- 长按3秒开关机
 */
void handle_key_button(void) {
    uint8_t key_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
    uint32_t current_time = HAL_GetTick();
    
    // 检测按键状态变化（假设按下为低电平）
    if(key_state == GPIO_PIN_RESET){
        // 按键按下
        if(button_was_pressed == 0){
            // 刚刚按下，记录时间
            button_press_start_time = current_time;
            button_was_pressed = 1;
            key_display_active = 1;  // 按下立即显示电量
        } else {
            // 持续按下，检查是否达到3秒
            if(current_time - button_press_start_time > 3000){
                // 长按3秒，切换开关机状态
                openfilg = !openfilg;
                button_press_start_time = current_time + 1000;  // 防止重复触发
            }
        }
    } else {
        // 按键松开
        if(button_was_pressed == 1){
            button_was_pressed = 0;
            key_display_active = 0;  // 松开关闭显示
        }
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
