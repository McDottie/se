/**
* @file		uart.h
* @brief	Contains the uart API.
* @version	1.1
* @date		18 Apr 2021
* @author	Jose Filipe Cruz dos Santos
*
*/
#ifndef UART_H_
#define UART_H_

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdbool.h>

#define UART_LSR_RDR 			(0x01)
#define UART_LSR_OE	 			(0x02)
#define UART_LSR_PE 			(0x04)
#define UART_LSR_FE 			(0x08)
#define UART_LSR_BI 			(0x10)
#define UART_LSR_THRE			(0x20)
#define UART_LSR_TEMT			(0x40)
#define UART_LSR_RXFE			(0x80)

#define UART_LCR_WLEN5			(0x00)
#define UART_LCR_WLEN6			(0x01)
#define UART_LCR_WLEN7			(0x02)
#define UART_LCR_WLEN8			(0x03)
#define UART_LCR_DLAB_EN 		(0x80)
#define UART_LCR_BREAK_EN 		(0x40)

#define UART_IIR_INTID_MASK 	(0x0E)
#define UART_IIR_INTSTAT_PEND 	(0x01)
#define UART_IIR_INTID_THRE		(0x02)
#define UART_IIR_INTID_RDA		(0x04)
#define UART_IIR_INTID_RLS		(0x06)
#define UART_IIR_INTID_CTI		(0x0C)

#define UART_IER_BITMASK		(0x07)
#define UART_IER_RBRINT_EN		(0x01)
#define UART_IER_THREINT_EN		(0x02)
#define UART_IER_RLSINT_EN		(0x04)

/// Ring buffer
#define UART_RBUFSIZE (1 << 11)
#define RBUF_MASK (UART_RBUFSIZE-1)
#define RBUF_IS_FULL(head, tail) ((tail&RBUF_MASK)==((head+1)&RBUF_MASK))
#define RBUF_WILL_FULL(head, tail) ((tail&RBUF_MASK)==((head+2)&RBUF_MASK))
#define RBUF_IS_EMPTY(head, tail) ((head&RBUF_MASK)==(tail&RBUF_MASK))
#define RBUF_RESET(bufidx) (bufidx=0)
#define RBUF_INCR(bufidx) (bufidx=(bufidx+1)&RBUF_MASK)

typedef struct {
	__IO uint32_t txWrite;
	__IO uint32_t txRead;
	__IO uint32_t rxWrite;
	__IO uint32_t rxRead;
	__IO uint8_t tx[UART_RBUFSIZE];
	__IO uint8_t rx[UART_RBUFSIZE];
} UART_RBUF_Type;

enum UART_Controller{
	UART0, UART1, UART2, UART3
};

enum UART_BaudRate{
	UART1200Hz = 1200,
	UART2400Hz = 2400,
	UART4800Hz = 4800,
	UART9600Hz = 9600,
	UART19200Hz = 19200,
	UART38400Hz = 38400,
	UART57600Hz = 57600,
	UART115200Hz = 115200,
};

bool UART_Initialize(enum UART_BaudRate baud,enum UART_Controller controller);

bool UART_IsChar(void);

unsigned char UART_ReadChar(void);

bool UART_GetChar(unsigned char *ch);

void UART_WriteChar(unsigned char ch);

void UART_WriteString(char *str);

void UART_Printf(char *fmt, ...);

uint32_t UART_WriteBuffer(unsigned char *buffer, int len);

uint32_t UART_ReadBuffer(uint8_t *buffer, uint32_t len);

uint32_t UART_ReadBufferAwait(uint8_t *buffer, uint32_t len);

void UART_Printf(char *format, ...);

#endif /* UART_H_ */
