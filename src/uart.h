/*
 * uart.h
 *
 * Project stm32sms
 *
 * Copyright (C) 2017 OOO NPK V-Real
 *
 *  Created on: May 11, 2017
 *      Author: Dmitry Saychenko <sadmitry@gmail.com>
 */

#ifndef UART_H_
#define UART_H_

#include <stm32f10x.h>
#include <stm32f10x_conf.h>

#define MODEM_USART		USART3
#define CLI_USART		USART1

#define MODEM_SPEED		115200
#define CLI_SPEED		57600

void InitUsart1(uint32_t speed);
void InitUsart2(uint32_t speed);
void InitUsart3(uint32_t speed);

void Init_Modem_Usart(void);
void Init_CLI_USART(void);

uint8_t CLI_Getchar(void);
void CLI_Send(uint8_t c);
void CLI_Print(const char * string);

char Modem_GetChar(void);
void Modem_Send(char c);
void Modem_Print(const char * string);
uint8_t GetModemCharReceivedFlag(void);

void Usart_2_Send(char c);
void Usart_3_Send(char c);

void Usart_2_Print(const char * string);
void Usart_3_Print(const char * string);

#endif /* UART_H_ */

