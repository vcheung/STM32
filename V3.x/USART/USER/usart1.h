#ifndef __USART1_H
#define __USART1_H

#include <stdio.h>
#include "stm32f10x.h"
#include <stdarg.h>	// USART1_printf������Щ��������������ļ�

void USART1_Config(void);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
