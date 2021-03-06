/*
 * uart.c
 *
 *  Created on: 18 Apr 2021
 *      Author: José Santos
 */
#include "FreeRTOS.h"
#include "queue.h"

#include "uart.h"
#include "utils.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


volatile LPC_UART_TypeDef *ctrl;
volatile bool intrTxStatus;
//volatile UART_RBUF_Type rbuffer;

QueueHandle_t xQueueRX;
QueueHandle_t xQueueTX;


void UARTSetDivisors(enum UART_BaudRate baud)
{
	/*
	int divisor = SystemCoreClock / baud;

	ctrl->DLL = (divisor & 0xFF);
	ctrl->DLM = (divisor>>8 & 0xFF);
	*/
	float bRest = 1;
	int bMulVal, bDivAddVal, bDivisor;

	for (int mulVal = 1; mulVal <= 15; mulVal++)
	{
		for (int divAddVal = 0; divAddVal < mulVal; divAddVal++)
		{
			float divisor = SystemCoreClock / (16 * baud * (1 + divAddVal/mulVal));
			
			float rest = divisor - (int)divisor;

			if(rest < bRest) {
				bRest = rest;
				bDivisor = (int)divisor;
				bDivAddVal = divAddVal;
				bMulVal = mulVal;
			}
		}
	}

	ctrl->LCR |= UART_LCR_DLAB_EN;
	
	ctrl->DLL = (bDivisor & 0xFF);
	ctrl->DLM = ((bDivisor>>8) & 0xFF);

	ctrl->FDR = (bMulVal>>8 & 0xF) | (bDivAddVal & 0xF);
}

bool UART_Initialize(enum UART_BaudRate baud, enum UART_Controller controller) {
	uint8_t tmp;
	IRQn_Type irq;
	switch(controller)
	{
		case UART0:
			UTILS_SetPinSelFunction(PINSEL0, 4, M01);	//TX0 P0.2
			UTILS_SetPinSelFunction(PINSEL0, 6, M01);	//RX0 P0.3

			UTILS_SetResistorMode(PINMODE0, 4, PULL_UP);
			UTILS_SetResistorMode(PINMODE0, 6, PULL_UP);

			UTILS_SetOpenDrainMode(PINMODE_OD0, 2, NORMAL);
			UTILS_SetOpenDrainMode(PINMODE_OD0, 3, NORMAL);

			LPC_SC->PCLKSEL0 |= (0x01 << 6);
			LPC_SC->PCONP |= (0x1 << 3);
			irq = UART0_IRQn;
			ctrl = LPC_UART0;
			break;
		case UART1:
			UTILS_SetPinSelFunction(PINSEL0, 30, M01);	//TX1 P0.15
			UTILS_SetPinSelFunction(PINSEL1, 0, M01);	//RX1 P0.16

			UTILS_SetResistorMode(PINMODE0, 30, PULL_UP);
			UTILS_SetResistorMode(PINMODE1, 0, PULL_UP);

			UTILS_SetOpenDrainMode(PINMODE_OD0, 15, NORMAL);
			UTILS_SetOpenDrainMode(PINMODE_OD0, 16, NORMAL);

			LPC_SC->PCLKSEL0 |= (0x01 << 8);
			LPC_SC->PCONP |= (0x1 << 4);
			irq = UART1_IRQn;
			ctrl = LPC_UART1;
			break;
		case UART2:
			UTILS_SetPinSelFunction(PINSEL0, 20, M01);	//TX2 P0.10
			UTILS_SetPinSelFunction(PINSEL0, 22, M01);	//RX2 P0.11

			UTILS_SetResistorMode(PINMODE0, 20, PULL_UP);
			UTILS_SetResistorMode(PINMODE0, 22, PULL_UP);

			UTILS_SetOpenDrainMode(PINMODE_OD0, 10, NORMAL);
			UTILS_SetOpenDrainMode(PINMODE_OD0, 11, NORMAL);

			LPC_SC->PCLKSEL1 |= (0x01 << 16);
			LPC_SC->PCONP |= (0x1 << 24);
			irq = UART2_IRQn;
			ctrl = LPC_UART2;
			break;
		case UART3:
			UTILS_SetPinSelFunction(PINSEL9, 24, M11);	//TX3 P4.28
			UTILS_SetPinSelFunction(PINSEL9, 26, M11);	//RX3 P4.29

			UTILS_SetResistorMode(PINMODE9, 24, PULL_UP);
			UTILS_SetResistorMode(PINMODE9, 26, PULL_UP);

			UTILS_SetOpenDrainMode(PINMODE_OD4, 28, NORMAL);
			UTILS_SetOpenDrainMode(PINMODE_OD4, 29, NORMAL);

			LPC_SC->PCLKSEL1 |= (0x01 << 18);
			LPC_SC->PCONP |= (0x1 << 25);
			irq = UART3_IRQn;
			ctrl = LPC_UART3;
			break;
	}

	// FIFOs are empty
	ctrl->FCR = ( 0x01 | 0x02 | 0x04);
	ctrl->FCR = 0; // Disable FIFO
	while (ctrl->LSR & UART_LSR_RDR) {
		tmp = ctrl->RBR;
	}

	ctrl->TER = 0x80;
	while (!(ctrl->LSR & UART_LSR_THRE)); // Wait for tx complete

	ctrl->TER = 0; // Disable Tx
	ctrl->IER = 0; // Disable interrupt
	ctrl->LCR = 0; // Set LCR to default state
	ctrl->ACR = 0; // Set ACR to default state

	if ((LPC_UART1_TypeDef *)ctrl == LPC_UART1) {
		((LPC_UART1_TypeDef *) ctrl)->MCR = 0; // Modem
		((LPC_UART1_TypeDef *) ctrl)->RS485CTRL = 0; // RS485
		((LPC_UART1_TypeDef *) ctrl)->RS485DLY = 0;
		((LPC_UART1_TypeDef *) ctrl)->ADRMATCH = 0;
		tmp = ((LPC_UART1_TypeDef *) ctrl)->MSR; // Clear
	}

	tmp = ctrl->LSR; // Clean status

	UARTSetDivisors(baud);

	//tmp = (ctrl->LCR & (UART_LCR_DLAB_EN | UART_LCR_BREAK_EN)) & 0xFF;
	tmp = UART_LCR_WLEN8;
	ctrl->LCR = (uint8_t) (tmp & 0xFF);
	ctrl->TER |= 0x80;

	ctrl->FCR = 0x00; //fifo enable with rx trigger level 0 (triggers on 1 character)
	ctrl->IER = UART_IER_RBRINT_EN | UART_IER_RLSINT_EN; //enable RBR interrupt and RX Line Status interrupt

	intrTxStatus = true;
	/*
	RBUF_RESET(rbuffer.rxWrite);
	RBUF_RESET(rbuffer.rxRead);
	RBUF_RESET(rbuffer.txWrite);
	RBUF_RESET(rbuffer.txRead);
	*/
	if(xQueueRX) vQueueDelete(xQueueRX);
	if(xQueueTX) vQueueDelete(xQueueTX);

	xQueueRX = xQueueCreate( UART_RBUFSIZE, sizeof(char));
	if(!xQueueRX) {
		xQueueRX = 1;
	}
	xQueueTX = xQueueCreate( UART_RBUFSIZE, sizeof(char));

	while (1) {
		uint32_t iir;
		iir = (ctrl->IIR & 0x03CF) & UART_IIR_INTID_MASK;
		if ((iir == UART_IIR_INTID_RDA) || (iir == UART_IIR_INTID_CTI)) {
			tmp = ctrl->RBR;
		}
		else break;
	}

	NVIC_EnableIRQ(irq);
	
	return true;
}

uint32_t UART_ReadBuffer(uint8_t *buffer, uint32_t len) {
	uint8_t *data = (uint8_t*) buffer;
	uint32_t bytes = 0;
	ctrl->IER &= (~UART_IER_RBRINT_EN) & UART_IER_BITMASK;
	bool notEmpty = true;

	while ((len > 0) && notEmpty ){//(!(RBUF_IS_EMPTY(rbuffer.rxWrite, rbuffer.rxRead)))) {
		//*data = rbuffer.rx[rbuffer.rxRead];
		notEmpty = xQueueReceive(xQueueRX, data, 0);
		if(!notEmpty) break;
		data++;
		//RBUF_INCR(rbuffer.rxRead);
		bytes++;
		len--;
	}

	ctrl->IER |= UART_IER_RBRINT_EN;
	return bytes;
}

uint32_t UART_WriteBuffer(unsigned char *buffer, int len) {
	uint8_t *data = (uint8_t*) buffer;
	uint32_t bytes = 0;
	ctrl->IER &= (~UART_IER_THREINT_EN) & UART_IER_BITMASK;
	while ((len > 0) && uxQueueSpacesAvailable(xQueueTX) != 0) {//(!RBUF_IS_FULL(rbuffer.txWrite, rbuffer.txRead))) {
		//rbuffer.tx[rbuffer.txWrite] = *data;
		xQueueSend(xQueueTX, data, 0);
		data++;
		//RBUF_INCR(rbuffer.txWrite);
		bytes++;
		len--;
	}
	
	//if(RBUF_IS_EMPTY(rbuffer.txWrite, rbuffer.txRead)) {
	if(uxQueueSpacesAvailable(xQueueTX) == UART_RBUFSIZE) {
		ctrl->IER &= (~UART_IER_THREINT_EN) & UART_IER_BITMASK; //remove transmit interrupt
		intrTxStatus = true;
		return bytes;
	}

	if(intrTxStatus) {
		intrTxStatus = false;


		//char tmp = rbuffer.tx[rbuffer.txRead];
		//RBUF_INCR(rbuffer.txRead);
		char tmp;
		xQueueReceive(xQueueTX, &tmp, 0);

		ctrl->THR = tmp;
		ctrl->IER |= UART_IER_THREINT_EN; //enable interrupt
	}
	return bytes;
}

bool UART_IsChar(void) 
{
	return uxQueueSpacesAvailable(xQueueRX) != 0; //!(RBUF_IS_EMPTY(rbuffer.rxWrite, rbuffer.rxRead));
}

unsigned char UART_ReadChar(void) 
{
	unsigned char c = 0;
	while (!UART_GetChar(&c));
	return c;
}

bool UART_GetChar(unsigned char *ch) 
{
	return UART_ReadBuffer(ch,1);
}

void UART_WriteChar(unsigned char ch) 
{
	UART_WriteBuffer(&ch,1);
}

void UART_WriteString(char *str) 
{
	UART_WriteBuffer(str,strlen(str));
}

void UART_Printf(char *fmt, ...) {
	char buffer[UART_RBUFSIZE/4];
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer, fmt, args);
	va_end(args);
	UART_WriteString(buffer);
}

uint32_t UART_ReadBufferAwait(uint8_t *buffer, uint32_t len) {
	int sum = 0;

	while(sum < len){
		int prevSum = sum;
		sum += UART_ReadBuffer(&buffer[sum], len-sum);

		WAIT_Milliseconds(100);
	}
	return sum;
}

void UART_IRQRoutine() {
	uint32_t iir;
	BaseType_t toYield;

	while (!((iir = ctrl->IIR) & UART_IIR_INTSTAT_PEND)) {
		switch (iir & UART_IIR_INTID_MASK) {
			case UART_IIR_INTID_RLS:
				if (ctrl->LSR & (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE))
				
				break;
			case UART_IIR_INTID_RDA:
			case UART_IIR_INTID_CTI:
				while ((ctrl->LSR & UART_LSR_RDR) != 0) {
					//rbuffer.rx[rbuffer.rxWrite] = ctrl->RBR;
					//RBUF_INCR(rbuffer.rxWrite);
					char tmp = ctrl->RBR;
					xQueueSendFromISR(xQueueRX, &tmp, &toYield);
				}
				break;
			case UART_IIR_INTID_THRE:
				ctrl->IER &= (~UART_IER_THREINT_EN) & UART_IER_BITMASK;
				char tmp;
				BaseType_t notEmpty = xQueueReceiveFromISR(xQueueTX, &tmp, &toYield);

				if (notEmpty){//!RBUF_IS_EMPTY(rbuffer.txWrite, rbuffer.txRead) && ((ctrl->LSR & UART_LSR_THRE) != 0)) {
					//ctrl->THR = rbuffer.tx[rbuffer.txRead];
					//RBUF_INCR(rbuffer.txRead);
					ctrl->THR = tmp;
					//notEmpty = xQueueReceiveFromISR(xQueueTX, &tmp, &toYield);
				}

				if(uxQueueSpacesAvailable(xQueueTX) == UART_RBUFSIZE) {
					ctrl->IER &= (~UART_IER_THREINT_EN) & UART_IER_BITMASK; //remove transmit interrupt
					intrTxStatus = true;
				} else {
					ctrl->IER |= UART_IER_THREINT_EN;
				}

				/*if(RBUF_IS_EMPTY(rbuffer.txWrite, rbuffer.txRead)) {
					ctrl->IER &= (~UART_IER_THREINT_EN) & UART_IER_BITMASK; //remove transmit interrupt
					intrTxStatus = true;
				} else {
					ctrl->IER |= UART_IER_THREINT_EN;
				}*/

				break;
		}
	}

	portYIELD_FROM_ISR(toYield);
}

void UART0_IRQHandler(void) {
    UART_IRQRoutine();
}

void UART1_IRQHandler(void) {
    UART_IRQRoutine();
}

void UART2_IRQHandler(void) {
    UART_IRQRoutine();
}

void UART3_IRQHandler(void) {
    UART_IRQRoutine();
}
