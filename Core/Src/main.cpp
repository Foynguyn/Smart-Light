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
#include <stdio.h>
#include <string.h>
#include "Fuzzy.h"
#include "fonts.h"
#include "ssd1306.h"
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
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
uint16_t light[2];
char tx_buffer[20];

Fuzzy *fuzzy = new Fuzzy();

// define membership
// inside room
FuzzySet *in_veryDark = new FuzzySet(0, 0, 10, 20);
FuzzySet *in_dark     = new FuzzySet(10, 20, 30, 40);
FuzzySet *in_moderate = new FuzzySet(30, 40, 50, 60);
FuzzySet *in_bright   = new FuzzySet(50, 60, 70, 80);
FuzzySet *in_veryBright = new FuzzySet(70, 80, 90, 100);

// outside
FuzzySet *out_veryDark = new FuzzySet(0, 0, 10, 20);
FuzzySet *out_dark     = new FuzzySet(10, 20, 30, 40);
FuzzySet *out_moderate = new FuzzySet(30, 40, 50, 60);
FuzzySet *out_bright   = new FuzzySet(50, 60, 70, 80);
FuzzySet *out_veryBright = new FuzzySet(70, 80, 90, 100);

// brightness
FuzzySet *veryLow	= new FuzzySet(0, 0, 10, 20);
FuzzySet *low		= new FuzzySet(10, 20, 30, 40);
FuzzySet *medium	= new FuzzySet(30, 40, 50, 60);
FuzzySet *high		= new FuzzySet(50, 60, 70, 80);
FuzzySet *veryHigh	= new FuzzySet(70, 80, 100, 100);


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void setupFuzzy();
void configureFuzzyRule();
void addRule(int ruleNumber, FuzzySet *fuzzySet1, FuzzySet *fuzzySet2,
		FuzzyRuleConsequent *Consequent);
float scaleLightValue(uint16_t light, uint16_t fromLow, uint16_t fromHigh,
		uint16_t toLow, uint16_t toHigh);
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

	SSD1306_Init(); // initialise the display

	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*) light, 2) != HAL_OK) {
		strcpy(tx_buffer, "ADC failed!\n\r");
		SSD1306_GotoXY(10, 10); // goto 10, 10
		SSD1306_Puts(tx_buffer, &Font_7x10, SSD1306_COLOR_WHITE); // print Hello
		SSD1306_UpdateScreen(); // update screen
	}

	setupFuzzy();
	configureFuzzyRule();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		float scaledP1 = scaleLightValue(light[0], 0, 4095, 0, 100);
		float scaledP2 = scaleLightValue(light[1], 0, 4095, 0, 100);

		float illuminationP1 = 100 - scaledP1;
		float illuminationP2 = 100 - scaledP2;

		fuzzy->setInput(1, illuminationP1);
		fuzzy->setInput(2, illuminationP2);

		// mờ hóa
		fuzzy->fuzzify();

		// giải mờ
		float brightnessLED = fuzzy->defuzzify(1);

		if (brightnessLED < 33.33) {
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, brightnessLED);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		} else if (brightnessLED < 66.67) {
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, brightnessLED);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, brightnessLED);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
		} else {
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, brightnessLED);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, brightnessLED);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, brightnessLED);
		}

		SSD1306_Clear();
		SSD1306_GotoXY(1, 0);
		sprintf(tx_buffer, "light 1: %.2f", illuminationP1);
		SSD1306_Puts(tx_buffer, &Font_7x10, SSD1306_COLOR_WHITE);

		SSD1306_GotoXY(1, 12);
		sprintf(tx_buffer, "light 2: %.2f", illuminationP2);
		SSD1306_Puts(tx_buffer, &Font_7x10, SSD1306_COLOR_WHITE);

		SSD1306_GotoXY(1, 22);
		sprintf(tx_buffer, "bright: %.2f%%", brightnessLED);
		SSD1306_Puts(tx_buffer, &Font_7x10, SSD1306_COLOR_WHITE);

		SSD1306_UpdateScreen();

		HAL_Delay(500);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 2;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 71;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 999;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void setupFuzzy() {
	FuzzyInput *inside = new FuzzyInput(1);
	inside->addFuzzySet(in_veryDark);
	inside->addFuzzySet(in_dark);
	inside->addFuzzySet(in_moderate);
	inside->addFuzzySet(in_bright);
	inside->addFuzzySet(in_veryBright);
	fuzzy->addFuzzyInput(inside);

	FuzzyInput *outside = new FuzzyInput(2);
	outside->addFuzzySet(out_veryDark);
	outside->addFuzzySet(out_dark);
	outside->addFuzzySet(out_moderate);
	outside->addFuzzySet(out_bright);
	outside->addFuzzySet(out_veryBright);
	fuzzy->addFuzzyInput(outside);

	FuzzyOutput *brightness = new FuzzyOutput(1);
	brightness->addFuzzySet(low);
	brightness->addFuzzySet(veryLow);
	brightness->addFuzzySet(medium);
	brightness->addFuzzySet(high);
	brightness->addFuzzySet(veryHigh);
	fuzzy->addFuzzyOutput(brightness);
}

void configureFuzzyRule() {
	// Định nghĩa các kết quả
	FuzzyRuleConsequent *thenVeryLow = new FuzzyRuleConsequent();
	FuzzyRuleConsequent *thenLow = new FuzzyRuleConsequent();
	FuzzyRuleConsequent *thenMedium = new FuzzyRuleConsequent();
	FuzzyRuleConsequent *thenHigh = new FuzzyRuleConsequent();
	FuzzyRuleConsequent *thenVeryHigh = new FuzzyRuleConsequent();

	// Cấu hình các kết quả
	thenVeryLow->addOutput(veryLow);
	thenLow->addOutput(low);
	thenMedium->addOutput(medium);
	thenHigh->addOutput(high);
	thenVeryHigh->addOutput(veryHigh);

	// Định nghĩa rule
	// Rule 1: If (outside bright) then Low
	addRule(1, out_bright, nullptr, thenLow);
	// Rule 2: If (outside very bright) then Low Power
	addRule(2, out_veryBright, nullptr, thenVeryLow);
	// Rule 3: If (outside moderate and inside very dark) then low Power
	addRule(3, out_moderate, in_veryDark, thenVeryHigh);

	addRule(4, out_moderate, in_dark, thenMedium);

	addRule(5, out_moderate, in_moderate, thenMedium);

	addRule(6, out_moderate, in_bright, thenVeryLow);

	addRule(7, out_moderate, in_veryBright, thenVeryLow);

	addRule(8, out_dark, in_veryDark, thenVeryHigh);

	addRule(9, out_dark, in_dark, thenMedium);

	addRule(10, out_dark, in_moderate, thenMedium);

	addRule(11, out_dark, in_bright, thenMedium);

	addRule(12, out_dark, in_veryBright, thenLow);

	addRule(13, out_veryDark, in_veryDark, thenVeryHigh);

	addRule(14, out_veryDark, in_dark, thenHigh);

	addRule(15, out_veryDark, in_moderate, thenHigh);

	addRule(16, out_veryDark, in_bright, thenMedium);

	addRule(17, out_veryDark, in_veryBright, thenMedium);
}

void addRule(int ruleNumber, FuzzySet *fuzzySet1, FuzzySet *fuzzySet2,
		FuzzyRuleConsequent *Consequent) {
	FuzzyRuleAntecedent *antecedent = new FuzzyRuleAntecedent();
	if (fuzzySet2 == nullptr) {
		antecedent->joinSingle(fuzzySet1);
		fuzzy->addFuzzyRule(new FuzzyRule(ruleNumber, antecedent, Consequent));
	} else {
		antecedent->joinWithAND(fuzzySet1, fuzzySet2);
		fuzzy->addFuzzyRule(new FuzzyRule(ruleNumber, antecedent, Consequent));
	}
}

float scaleLightValue(uint16_t light, uint16_t fromLow, uint16_t fromHigh,
		uint16_t toLow, uint16_t toHigh) {
	return ((light * 1.0 - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
