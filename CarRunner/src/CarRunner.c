/*
===============================================================================
 Name        : CarRunner.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"

#include <cr_section_macros.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lcd.h"
#include "time_helper.h"
#include "button.h"
#include "rtc.h"
#include "wait.h"
#include "ADXL345.h"
#include "spi.h"
#include "ESP01.h"
#include "NTP.h"
#include "saver.h"
#include "CarRunner.h"

#include <definitions_variables.h>


#define PRESSING_TIME 2000

QueueHandle_t xQueueRunGame,xQueueRunMenu,xQueueLCD,xQueueButton;
volatile key_state keySt = {0,RELEASED};

volatile int conStatus = 0;
volatile bool toUpdateTime = false;

char * menu_options[3] = {"Change Time","Show Players\nScores", "Exit"};

void routineChooser(int routine){
	switch(routine) {
	case 0:
		TIME_ChangeRoutine(RTC_GetSeconds());
		break;
	case 1:
		playerScoresShowDown();
		break;
	default:
		break;
	}
}

/**
 @startuml
	[*] -> Menu
	title CarRunner Menu

	Menu --> Menu : keypressed == L | R
	Menu : ShowMenuOption
	Menu --> ChooseOption : keypressed = S
	ChooseOption --> Time_changeRoutine
	ChooseOption --> playerScoresShowdown
	Menu --> [*]
 @enduml
*/
void Menu() {

	int i = 0;

	printSimple(menu_options[i],100);

	while(1){
		if(keySt.state == PRESSED)
		switch(keySt.key) {
			case S:
				if (i == 2)
					return;

				routineChooser(i);
				printSimple(menu_options[i],100);
				break;
			case L:
				i--;
				if (i < 0) i = 2;
				printSimple(menu_options[i],100);
				break;
			case R:
				i++;
				if (i > 2) i = 0;
				printSimple(menu_options[i],100);
				break;
			default:
				break;
		}
	}

}

/**
 @startuml
	[*] -> playerScoresShowdown
	title CarRunner Player Showdown
	playerScoresShowdown --> nextPlayer : keypressed == R
	playerScoresShowdown --> previousPlayer : keypressed == L
	previousPlayer --> playerScoresShowdown
	nextPlayer --> playerScoresShowdown

	playerScoresShowdown --> [*] : keypressed = S

@enduml
*/
void playerScoresShowDown(){

	int size = listSize();
	if(size == 0) {
		LCDText_Clear();
		LCDText_WriteString("NO PLAYERS\nSCORES YET");
		WAIT_Milliseconds(3000);
		return;
	}


	int * scores = readScores();
	char * strings[size];
	readNames(strings);


	key_state keyState;
	int i = 0;


	LCDText_Clear();
	LCDText_Printf("%s\n%d",strings[i],scores[i]);

	while(1){
		keyState = BUTTON_GetButtonsEvents();
		if(keyState.state == PRESSED)
		switch(keyState.key) {
			case S:
				return;
			case L:
				i--;
				if (i < 0) i = size-1;
				LCDText_Clear();
				LCDText_Printf("%s\n%d",strings[i],scores[i]);

				break;
			case R:
				i++;
				if (i >= size) i = 0;
				LCDText_Clear();
				LCDText_Printf("%s\n%d",strings[i],scores[i]);
				break;
			default:
				break;
		}
	}
	for(int i = 0; i < size; i++) {
		vPortFree(strings[i]);
	}
	vPortFree(scores);
}


void vMainLoop(void * pvParameters){
	//GAME_Init();
	SPI_Init();

	while(ADXL345_Init() == -1);

	int start;

	//time Update
	toUpdateTime = true;

	while(1) {
		int message;

		switch(keySt.key) {
			case S:
				if(keySt.state == PRESSED) {
					message = 1;
					toUpdateTime = false;
					xQueueSend(xQueueRunGame,(void *)&message,0);
					//gameRoutine();

					xQueueReceive(xQueueRunGame, &message, portMAX_DELAY);
					toUpdateTime = true;
				}

				break;
			case L|R:
				if (keySt.state == PRESSED)
					start = WAIT_GetElapsedMillis(0);
				if (keySt.state == REPEATED && WAIT_GetElapsedMillis(start) >= PRESSING_TIME) {
					message = 1;
					toUpdateTime = false;
					xQueueSend(xQueueRunMenu,(void *)&message,0);
					//Menu();
					xQueueReceive(xQueueRunMenu, &message, portMAX_DELAY);
					toUpdateTime = true;
				}
				break;
			default:
				break;
		}

	}
	return 0;
}

void vButtonHandler(void * pvParameters){
	key_state prev = {0, RELEASED};
	int timeStamp = -1;
	while(1)  {
		key_state keySt = BUTTON_GetButtonsEvents();
		if(keySt.key != prev.key || keySt.state != prev.state) {
			if(keySt.state == REPEATED) {
				if(timeStamp == -1) {
					timeStamp = WAIT_GetElapsedMillis(0);
				} else if(WAIT_GetElapsedMillis(timeStamp) > 100) {//TODO CREATE CONSTANT
					timeStamp == -1;
					xQueueSend(xQueueButton, &keySt, 0);
					prev = keySt;
				}
			} else {
				xQueueSend(xQueueButton, &keySt, 0);
				prev = keySt;
			}
		}
	}
}

void vGame(void * pvParameters){
	int message;
	while(1) {
		xQueueReceive(xQueueRunGame, &message, portMAX_DELAY);
		GAME_Routine();
	}
}

void vMenu(void * pvParameters){
	int message;
	while(1) {
		xQueueReceive(xQueueRunMenu, &message, portMAX_DELAY);
		Menu();
	}
}

void vLCDManager(void * pvParameters){
	LCDText_Init();
	LCDText_CursorOn();

	LCD_Mesage message;
	while(1){
		if(xQueueReceive(xQueueLCD, &message, portMAX_DELAY)) {
			if(message.toClear) {
				LCDText_Clear();
			}

			if(message.toMove) {
				LCDText_Locate(message.row, message.col);
			}

			if(message.toPrint) {
				LCDText_WriteString(message.string);
				vPortFree(message.string);
			}

		}
	}
}

void vTimeHandler(){

	while(conStatus != CON);

	NTP_Init(NTP_DEFAULT_SERVER, NTP_DEFAULT_PORT);

	RTC_Init(NTP_Request(5));

	int dateTime_Stamp = WAIT_GetElapsedMillis(0);
	struct tm dateTime;

	while(1) {
		if(toUpdateTime && WAIT_GetElapsedMillis(dateTime_Stamp) >= 1000) {
			dateTime_Stamp = WAIT_GetElapsedMillis(0);
			RTC_GetValue(&dateTime);
			TIME_UpdateDateTimeDisplay(dateTime);
		}
	}
}


void vConnection() {
	ESP01_Init();
	conStatus = STARTING;
	bool res = ESP01_Echo(0);
	while(!ESP01_Test());
	while(!ESP01_SetMode(STATION));
	while(!ESP01_ConnectAP("HUAWEI", "12345678"));
	ESP01_SetDNS("1.1.1.1","1.0.0.1");
	conStatus = CON;

	while(1) {

	}
}

/**
 @startuml
	[*] -> initPeripherals
	title CarRunner Main loop

	initPeripherals --> waitEvent
	waitEvent --> waitEvent : after 1s
	waitEvent --> Game : keypressed == S and PRESSED
	waitEvent --> Menu : keypressed == L&R for 2s
	waitEvent : update_DateTimeDisplay
	Game --> waitEvent
	Menu --> waitEvent

 @enduml
*/
int main(void) {
	WAIT_Init();

	xQueueRunGame = xQueueCreate(1,sizeof(int));
	xQueueRunMenu = xQueueCreate(1,sizeof(int));
	xQueueLCD = xQueueCreate(10,sizeof(LCD_Mesage));
	xQueueButton = xQueueCreate(20,sizeof(key_state));

	xTaskCreate(vConnection, "Connection", 600, (void *) 0, 1, NULL);

	xTaskCreate(vMainLoop, "MainLoop", 400, (void *) 0, 1, NULL);
	xTaskCreate(vButtonHandler, "ButtonHandler", 200, (void *) 0, 1, NULL);

	xTaskCreate(vGame, "Game", 200, (void *) 0, 1, NULL);
	xTaskCreate(vMenu, "Menu", 200, (void *) 0, 1, NULL);

	xTaskCreate(vLCDManager, "LCDManager", 400, (void *) 0, 1, NULL);
	xTaskCreate(vTimeHandler, "TimeHandler", 300, (void *) 0, 1, NULL);

	vTaskStartScheduler();
	return 0 ;
}


/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack. */

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* This function configures a timer that is used as the time base when
	collecting run time statistical information - basically the percentage
	of CPU time that each task is utilising.  It is called automatically when
	the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
	to 1). */

	/* Power up and feed the timer. */
	LPC_SC->PCONP |= (1 << 2);
	LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<4))) | (0x01 << 4);

	/* Reset Timer 1 */
	LPC_TIM1->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	LPC_TIM1->CTCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
	but not too fast so as to overflow all the time. */
	LPC_TIM1->PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

	/* Start the counter. */
	LPC_TIM1->TCR = TCR_COUNT_ENABLE;
}
/*-----------------------------------------------------------*/
