#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Подключаем FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#include "crc16.h"
#include "microrl.h"
#include "microrl_callbacks.h"
#include "utils.h"
#include "uart.h"

// CLI Commands
#define CMD_PRINT		0xE8A6
#define CMD_HELP		0x9B20

#define TaskLED1_Priority		tskIDLE_PRIORITY
#define TaskCLI_Priority		tskIDLE_PRIORITY+2

// create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;

void led_init()
{

	/* Включаем тактирование порта B */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Pin = GPIO_Pin_5;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_ResetBits( GPIOB, GPIO_Pin_5 );

	/* Инициализируем GPIO на порту B */
	GPIO_Init(GPIOB, &gpio);
}

/* Temporary task. Demo only */
void vTaskLED1(void *pvParameters) {

	for (;;) {
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		vTaskDelay(200);
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		vTaskDelay(300);
	}
}

void vApplicationTickHook( void )
{
}

void help(void){
	CLI_Print("Available commands:\r\n");
	CLI_Print("help\tPrint this text\r\n");
}

void Test_Print(const char *usart, const char *string){
	int usart_num = atoi(usart);
	switch(usart_num){
	case 1:
		CLI_Print(string);
		break;
	case 2:
		Usart_2_Print(string);
		break;
	case 3:
		Usart_3_Print(string);
		break;
	default:
		CLI_Print("Unknown USART");
		break;
	}
}

/*********************/
/* MicroRL callbacks */
/*********************/

int execute (int argc, const char * const * argv){

	uint16_t crc;

	CLI_Print("\r\n");

	if(argc > 0){

		crc = gen_crc16((const uint8_t *)argv[0], strlen(argv[0]));

		switch (crc) {
		case CMD_PRINT:
			if(3 == argc){
				Test_Print(argv[1], argv[2]);
			}else{
				CLI_Print("Valid format: print usart_number string");
			}
			break;
		case CMD_HELP:
			help();
			break;
		default:
			CLI_Print("Unknown command");
			break;
		}

	}else{
		CLI_Print("Armon>");
	}

	CLI_Print("\r\n");

	return 0;
}

/**
 * @brief Microrl task
 * @return
 */
void clitask(void *pvParameters){
	while (1) {
		// put received char from USART to microrl lib
		microrl_insert_char(prl, CLI_Getchar());
	}
}

/**
 * @brief Main entry point
 * @return
 */
int main( void )
{

	led_init();
	Init_CLI_USART();

	// call init with ptr to microrl instance and print callback
	microrl_init (prl, CLI_Print);
	// set callback for execute
	microrl_set_execute_callback (prl, execute);

	xTaskCreate( vTaskLED1, ( char * ) "LED1", configMINIMAL_STACK_SIZE, NULL, TaskLED1_Priority,
			( TaskHandle_t * ) NULL);
	xTaskCreate( clitask, ( char * ) "CLI", configMINIMAL_STACK_SIZE * 2, NULL, TaskCLI_Priority,
			( TaskHandle_t * ) NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	for(;;){}

	return 0;
}

