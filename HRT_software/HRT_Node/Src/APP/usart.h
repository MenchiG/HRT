#ifndef __USART1_H__
#define	__USART1_H__

#include "includes.h"
#include  <stdarg.h>
void USART1_Configuration(u32 baud);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */