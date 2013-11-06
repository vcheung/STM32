#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

void NVIC_Configuration(void);
void RTC_Configuration(void);
uint8_t USART_Scanf(uint32_t value);
uint32_t Time_Regulate(void);
void Time_Adjust(void);
void Time_Display(uint32_t TimeVar);
void Time_Show(void);

#endif	/* __RTC_H */
