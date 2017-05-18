/*
 * uart.c
 *
 * Project stm32sms
 *
 * Copyright (C) 2017 OOO NPK V-Real
 *
 *  Created on: May 11, 2017
 *      Author: Dmitry Saychenko <sadmitry@gmail.com>
 */

#include "uart.h"

void InitUsart1(uint32_t speed){
	GPIO_InitTypeDef gpio_port;

	//Включаем порты и UART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

	// Настраиваем ногу PA10 как вход UARTа (RxD)
	gpio_port.GPIO_Pin   = GPIO_Pin_10;
	gpio_port.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio_port);

	// Настраиваем ногу PA9 как выход UARTа (TxD)
	// Причем не просто выход, а выход с альтернативной функцией
	gpio_port.GPIO_Pin   = GPIO_Pin_9;
	gpio_port.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_port.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_port);

	//Заполняем структуру настройками UARTa
	USART_InitTypeDef uart_struct;
	uart_struct.USART_BaudRate            = speed;
	uart_struct.USART_WordLength          = USART_WordLength_8b;
	uart_struct.USART_StopBits            = USART_StopBits_1;
	uart_struct.USART_Parity              = USART_Parity_No ;
	uart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_struct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	//Инициализируем UART
	USART_Init(USART1, &uart_struct);

	//Включаем UART
	USART_Cmd(USART1, ENABLE);
}

void InitUsart3(uint32_t speed){
	GPIO_InitTypeDef gpio_port;
	//Включаем порты и UART3
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);

	// Настраиваем ногу PB11 как вход UARTа (RxD)
	gpio_port.GPIO_Pin   = GPIO_Pin_11;
	gpio_port.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &gpio_port);

	// Настраиваем ногу PB10 как выход UARTа (TxD)
	// Причем не просто выход, а выход с альтернативной функцией
	gpio_port.GPIO_Pin   = GPIO_Pin_10;
	gpio_port.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_port.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpio_port);

	//Заполняем структуру настройками UARTa
	USART_InitTypeDef uart_struct;
	uart_struct.USART_BaudRate            = speed;
	uart_struct.USART_WordLength          = USART_WordLength_8b;
	uart_struct.USART_StopBits            = USART_StopBits_1;
	uart_struct.USART_Parity              = USART_Parity_No ;
	uart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_struct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	//Инициализируем UART3
	USART_Init(USART3, &uart_struct);

	//Включаем UART
	USART_Cmd(USART3, ENABLE);
}

void InitUsart2(uint32_t speed){
	GPIO_InitTypeDef gpio_port;
	//Включаем порты и USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// Настраиваем ногу PA3 как вход UARTа (RxD)
	gpio_port.GPIO_Pin   = GPIO_Pin_3;
	gpio_port.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio_port);

	// Настраиваем ногу PA2 как выход UARTа (TxD)
	// Причем не просто выход, а выход с альтернативной функцией
	gpio_port.GPIO_Pin   = GPIO_Pin_2;
	gpio_port.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_port.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_port);

	//Заполняем структуру настройками UARTa
	USART_InitTypeDef uart_struct;
	uart_struct.USART_BaudRate            = speed;
	uart_struct.USART_WordLength          = USART_WordLength_8b;
	uart_struct.USART_StopBits            = USART_StopBits_1;
	uart_struct.USART_Parity              = USART_Parity_No ;
	uart_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_struct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	//Инициализируем UART
	USART_Init(USART2, &uart_struct);

	//Включаем UART
	USART_Cmd(USART2, ENABLE);
}

void Init_Modem_Usart(void){
	InitUsart3(MODEM_SPEED);
}

void Init_CLI_USART(void){
	InitUsart1(CLI_SPEED);
}

uint8_t CLI_Getchar(void){
	while(!(CLI_USART->SR & USART_SR_RXNE));
	return CLI_USART->DR;
}

void CLI_Send(uint8_t c){
	while(!(CLI_USART->SR & USART_SR_TC)); //Ждем пока бит TC в регистре SR станет 1
	CLI_USART->DR = c; //Отсылаем байт через UART
}

void CLI_Print(const char * string){
	uint8_t i=0;
	while(string[i]) {
		CLI_Send(string[i]);
		i++;
	}
}

char Modem_GetChar(void){
	while(!(MODEM_USART->SR & USART_SR_RXNE));
	return MODEM_USART->DR;
}

void Modem_Send(char c){
	while(!(MODEM_USART->SR & USART_SR_TC)); //Ждем пока бит TC в регистре SR станет 1
	MODEM_USART->DR = c; //Отсылаем байт через UART
}

void Modem_Print(const char * string){
	uint8_t i=0;
	while(string[i]) {
		Modem_Send(string[i]);
		i++;
	}
}

uint8_t GetModemCharReceivedFlag(void){
	return MODEM_USART->SR & USART_SR_RXNE;
}

void Usart_2_Send(char c){
	while(!(USART2->SR & USART_SR_TC)); //Ждем пока бит TC в регистре SR станет 1
	USART2->DR = c; //Отсылаем байт через UART
}

void Usart_3_Send(char c){
	while(!(USART3->SR & USART_SR_TC)); //Ждем пока бит TC в регистре SR станет 1
	USART3->DR = c; //Отсылаем байт через UART
}

void Usart_2_Print(const char * string){
	uint8_t i=0;
	while(string[i]) {
		Usart_2_Send(string[i]);
		i++;
	}
}

void Usart_3_Print(const char * string){
	uint8_t i=0;
	while(string[i]) {
		Usart_3_Send(string[i]);
		i++;
	}
}

