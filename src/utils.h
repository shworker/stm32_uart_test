/*
 * utils.h
 *
 * Project stm32_eth_test1
 *
 * Copyright (C) 2015 OOO NPK V-Real
 *
 *  Created on: Dec 10, 2015
 *      Author: Dmitry Saychenko <sadmitry@gmail.com>
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define HEX_BYTE_LENGTH		8

uint8_t int2hex(uint32_t decimalNumber, char *hexadecimalNumber);
uint8_t dig2ascii(uint8_t dig);
uint32_t hex2int(char *string);
int myatoi(const char *s);
char* itoa(int num, char* str, int base);
void reverse(char str[], int length);
void swap(char *a, char *b);

#endif /* UTILS_H_ */
