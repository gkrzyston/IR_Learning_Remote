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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct Commands {
	uint8_t UART_Data;
	/*
	 * The full description is used in the CLI and also
	 * displayed on the touchscreen.
	*/
	char full_desc[16];
	/*
	 * The shortened description is used on the button array displays.
	*/
	char desc[5];
} Command;

typedef struct Devices {
	/*
	 * Commands contain the UART_Data to be sent to devices
	 * Command 0 is the power toggle
	 * Commands 1-9 are accessed on the button array
	 * Commands 10-15 are on the first page of the touchscreen
	 * Commands 16-21 are on the second page
	 * Commands 22-27 on the third page
	*/
	Command commands[28];
	/*
	 * The devices name is printed at the top of the touchscreen
	 * and is also used to identify devices in the CLI
	*/
	char name[32];
	/*
	 * The number of buttons associated with this device
	*/
	uint8_t num_button_commands;
	uint8_t num_touch_commands;
} Device;

typedef struct Controllers {
	Device devices[16];
	uint8_t num_devices;
} Controller;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
const char* deliminator = "\n";
const char* section_deliminator = ",";
char UART1_rxBuffer[8192] = {'\0'};
uint8_t sync = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void print_device(uint8_t device, Controller* controller);
void print_devices(Controller* controller);
void print_command(uint8_t device, uint8_t command, Controller* controller);
void print_commands(uint8_t device, Controller* controller);
void swap_device(uint8_t a, uint8_t b, Controller* controller);
void rename_device(char* new_name, uint8_t device, Controller* controller);
void add_device(Controller* controller);
void delete_device(uint8_t device, Controller* controller);
void export_configuration(Controller* controller);

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
	Controller controller;
	controller.num_devices = 0;
	// TODO: Restore Saved Devices

	// Initial Case: No saved devices
	if (!controller.num_devices) {
		strcpy(controller.devices[0].commands[0].desc, "PWR");
		strcpy(controller.devices[0].commands[0].full_desc, "Power Toggle");
		controller.devices[0].commands[0].UART_Data = 0x7B;
		char buf[3];
		for (uint8_t i = 1; i <= 9; ++i) {
			itoa(i < 5 ? i : i + 5,buf,10);
			strcpy(controller.devices[0].name, "Example Device 0");
			strcpy(controller.devices[0].commands[i < 5 ? i : i + 5].desc, "CM");
			strncat(controller.devices[0].commands[(i < 5 ? i : i + 5)].desc, buf,strlen(buf));
			strcpy(controller.devices[0].commands[i < 5 ? i : i + 5].full_desc, "Long Desc ");
			strncat(controller.devices[0].commands[(i < 5 ? i : i + 5)].full_desc, buf,strlen(buf));
			controller.devices[0].commands[i < 5 ? i : i + 5].UART_Data = i < 5 ? i : i + 5;
		}
		controller.num_devices = 1;
		controller.devices[0].num_button_commands = 5;
		controller.devices[0].num_touch_commands = 5;

		strcpy(controller.devices[1].commands[0].desc, "PWR");
		strcpy(controller.devices[1].commands[0].full_desc, "Power Toggle");
		controller.devices[1].commands[0].UART_Data = 0x4A;
		for (uint8_t i = 1; i <= 8; ++i) {
			itoa(i < 5 ? i : i + 5,buf,10);
			strcpy(controller.devices[1].name, "Example Device 1");
			strcpy(controller.devices[1].commands[i < 5 ? i : i + 5].desc, "CM");
			strncat(controller.devices[1].commands[(i < 5 ? i : i + 5)].desc, buf,strlen(buf));
			strcpy(controller.devices[1].commands[i < 5 ? i : i + 5].full_desc, "Long Desc ");
			strncat(controller.devices[1].commands[(i < 5 ? i : i + 5)].full_desc, buf,strlen(buf));
			controller.devices[1].commands[i < 5 ? i : i + 5].UART_Data = i < 5 ? i : i + 5;
		}
		controller.num_devices = 2;
		controller.devices[1].num_button_commands = 5;
		controller.devices[1].num_touch_commands = 4;
	}

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  char start_message[] = "Successfully Booted.";
  HAL_UART_Transmit(&huart1, (uint8_t *)start_message, strlen(start_message), 100);
  HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (!sync) {
    	sync = 1;
    	HAL_UART_Transmit(huart, UART1_rxBuffer, 1, 100);
    	HAL_UART_Receive_IT(huart, UART1_rxBuffer, 4);
    	return;
    }

    UART1_rxBuffer[5] = '\0';

    if (!strcmp(UART1_rxBuffer,"quit")) {
    	sync = 0;
    	HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 1);
    	return;
    } else if (!strcmp(UART1_rxBuffer,"econ")) {
    	// If the host wants to export, client needs to import
    	//import_configuration();
    } else if (!strcmp(UART1_rxBuffer,"icon")) {
    	// If the host wants to import, client needs to export
    	//export_configuration();
    	char message[] = "I just sent a big packet!";
    	HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), 100);
    }

    HAL_UART_Receive_IT(huart, UART1_rxBuffer, 4);
}

void print_number(uint8_t n) {
	char buf[3];
	itoa(n,buf,10);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_device(uint8_t device, Controller* controller) {
	HAL_UART_Transmit(&huart1, (uint8_t*)(controller->devices[device].name),strlen(controller->devices[device].name),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_devices(Controller* controller) {
	for (uint8_t i = 0; i < controller->num_devices; ++i) {
		PrintDevice(i,controller);
	}
}

void print_command(uint8_t device, uint8_t command, Controller* controller) {
	char data_buffer[3];
	itoa(controller->devices[device].commands[command].UART_Data, data_buffer, 16);

	HAL_UART_Transmit(&huart1, (uint8_t*)(controller->devices[device].commands[command].full_desc),strlen(controller->devices[device].commands[command].full_desc),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)section_deliminator, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)(controller->devices[device].commands[command].desc),strlen(controller->devices[device].commands[command].desc),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)section_deliminator, 1, 0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)data_buffer,strlen(data_buffer),0xFFFF);
	HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
}

void print_button_commands(uint8_t device, Controller* controller) {
	for (uint8_t i = 0; i < controller->devices[device].num_button_commands; ++i) {
		print_command(device, i, controller);
	}
}

void print_touch_commands(uint8_t device, Controller* controller) {
	for (uint8_t i = 10; i < 10 + controller->devices[device].num_touch_commands; ++i) {
		print_command(device, i, controller);
	}
}

void print_commands(uint8_t device, Controller* controller) {
	print_button_commands(device, controller);
	print_touch_commands(device, controller);
}

void add_device(Controller* controller) {
	char buf[3];
	char msg[20];
	itoa(controller->num_devices,buf,10);
	strcpy(msg, "Device ");
	strncat(msg, buf, strlen(buf));
	strcpy(controller->devices[controller->num_devices].name,msg);
	controller->devices[controller->num_devices].num_button_commands = 0;
	controller->devices[controller->num_devices].num_touch_commands = 0;
	++(controller->num_devices);
}

void delete_device(uint8_t device, Controller* controller) {
	--(controller->num_devices);
	for (uint8_t i = device; i < controller->num_devices; ++i) {
		swap_device(i, i+1, controller);
	}
}

/*
 * Device Export Format:
 * Device Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 */
void export_device(uint8_t device, Controller* controller) {
	print_device(device, controller);

	print_number(controller->devices[device].num_button_commands);
	print_button_commands(device, controller);

	print_number(controller->devices[device].num_touch_commands);
	print_touch_commands(device, controller);
}
/*
 * Device Import Format:
 * Device Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 */
void import_device();

void rename_device(char* new_name, uint8_t device, Controller* controller) {
	strcpy(controller->devices[device].name, new_name);
}
void swap_device(uint8_t a, uint8_t b, Controller* controller) {
	Device tmp = controller->devices[a];
	controller->devices[a] = controller->devices[b];
	controller->devices[b] = tmp;
}

void add_command();
void set_command();
void delete_command();
void rename_command();
void swap_command();
void copy_command();

/*
 * Configuration Export Format:
 * # of Devices
 * Device #1 Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * (blank line)
 * Device #2 Name
 * ...
*/
void export_configuration(Controller* controller) {

	print_number(controller->num_devices);

	for (uint8_t i = 0; i < controller->num_devices; ++i) {
		export_device(i, controller);
		HAL_UART_Transmit(&huart1, (uint8_t*)deliminator, 1, 0xFFFF);
	}
}
/*
 * Configuration Export Format:
 * # of Devices
 * Device #1 Name
 * Number of Button Commands (including pwr)
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * ...
 * Number of Touch Commands
 * Command #1 Long Name, Short Name, Data
 * Command #2 Long Name, Short Name, Data
 * (blank line)
 * Device #2 Name
 * ...
*/
void import_configuration();

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
