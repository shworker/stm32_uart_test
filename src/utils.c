/*
 * utils.c
 *
 * Project stm32_eth_test1
 *
 * Copyright (C) 2015 OOO NPK V-Real
 *
 *  Created on: Dec 10, 2015
 *      Author: Dmitry Saychenko <sadmitry@gmail.com>
 */
#include <stm32f10x.h>
#include <stm32f10x_conf.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
//#include <stdbool.h>
#include "utils.h"
#include "uart.h"

uint8_t int2hex(uint32_t decimalNumber, char *hexadecimalNumber){
	uint32_t quotient;
	uint8_t i = 0;
	uint8_t temp;
	quotient = decimalNumber;
	while(quotient != 0) {
		temp = quotient % 16;
		//To convert integer into character
		if( temp < 10)
			temp = temp + 48;
		else
			temp = temp + 55;
		hexadecimalNumber[i++]= temp;
		quotient = quotient / 16;
	}
	return i;
}

uint8_t dig2ascii(uint8_t dig){
	return (0x30 + dig);
}

uint32_t hex2int(char *string){
	uint32_t number;
	if(('0' == string[0]) && ('x' == string[1])){
		// string begin with 0x
		number = (uint32_t)strtol(string, NULL, 0);
	}else{
		number = (uint32_t)strtol(string, NULL, 16);
	}
	return number;
}

int myatoi(const char *s)
{
	int i, n=0;
	for(i=0; s[i]>='0' && s[i]<='9'; i++)
		n = 10*n + (s[i] - '0');
	return n;
}

void swap(char *a, char *b)
{
	char z = *a;
	*a = *b;
	*b = z;
}

/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((str+start), (str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = FALSE;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = TRUE;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

void print_hex(uint16_t data){
	char hexbyte[HEX_BYTE_LENGTH];
	memset(hexbyte, 0, HEX_BYTE_LENGTH);
	int2hex(data, hexbyte);
	CLI_Print(hexbyte);
}
