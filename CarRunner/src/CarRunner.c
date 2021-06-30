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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lcd.h"
#include "time_helper.h"
#include "button.h"
#include "rtc.h"
#include "wait.h"
#include "ADXL345.h"
#include "i2c.h"
#include "spi.h"
#include "ESP01.h"

#include "network.h"
#include <mqtt.h>
#include "saver.h"
#include "game.h"
#include "CarRunner.h"

#include <definitions_variables.h>


QueueHandle_t xQueueLCD,xQueueButton, xQueueMqtt;

volatile bool gameRunning, menuRunning;
volatile int conStatus = 0;
volatile bool toUpdateTime = false;

char * menu_options[3] = {"Change Time","Show Players\nScores", "Exit"};

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

	int size = SAVER_ListSize();
	if(size == 0) {
		char * string = pvPortMalloc(22);
		strcpy(string,"NO PLAYERS\nSCORES YET");
		printComplex(10, true, false, true, false, 0, 0,string);
		WAIT_Milliseconds(3000);
		return;
	}


	SAVER_Score ** scores = SAVER_ReadScores();
	int i = 0;

	printfComplex(10, true, false, true, false, 0, 0,"%s\n%d",scores[i]->username,scores[i]->score);

	key_state keySt;

	while(1) {

		if(!xQueueReceive(xQueueButton, &keySt, 100)) {
			continue;
		}

		if(keySt.state == PRESSED)
		switch(keySt.key) {
			case S:
				return;
			case L:
				i--;
				if (i < 0) i = size-1;
				printfComplex(10, true, false, true, false, 0, 0,"%s\n%d",scores[i]->username,scores[i]->score);
				break;
			case R:
				i++;
				if (i >= size) i = 0;
				printfComplex(10, true, false, true, false, 0, 0,"%s\n%d",scores[i]->username,scores[i]->score);
				break;
			default:
				break;
		}
	}

	SAVER_CleanScores(scores,size);

	vPortFree(scores);
}

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

	printSimple(100, menu_options[i]);

	key_state keySt;

	while(1) {

		if(!xQueueReceive(xQueueButton, &keySt, 100)) {
			continue;
		}

		if(keySt.state == PRESSED)
		switch(keySt.key) {
			case S:
				if (i == 2)
					return;

				routineChooser(i);
				printSimple(100, menu_options[i]);
				break;
			case L:
				i--;
				if (i < 0) i = 2;
				printSimple(100, menu_options[i]);
				break;
			case R:
				i++;
				if (i > 2) i = 0;
				printSimple(100, menu_options[i]);
				break;
			default:
				break;
		}
	}

}


void vMainLoop(void * pvParameters){
	I2C_Init(I2C1_ALT);

	int start;
	int elapsedAnimation = WAIT_GetElapsedMillis(0);
	key_state keySt;
	int cnt = 0;
	char *strings[3] = {"Connecting .","Connecting ..","Connecting ..."};

	while (1) {
		int message;

		if (!gameRunning && !menuRunning) {
			toUpdateTime = true;
			if (conStatus == DISCON || conStatus == STARTING) {
				if (WAIT_GetElapsedMillis(elapsedAnimation) > CONNECTING_ANIMATION_DELAY) {
					elapsedAnimation = WAIT_GetElapsedMillis(0);
					cnt = cnt >= 3 ? 0 : cnt;

					char * string = pvPortMalloc(14);
					strcpy(string,strings[cnt]);
					printSimple(100, string);
					cnt++;
				}
			}
			if (!xQueueReceive(xQueueButton, &keySt, 100)) {
				continue;
			}

			switch (keySt.key) {
				case S:
					if(keySt.state == PRESSED) {
						message = 1;
						toUpdateTime = false;
						gameRunning = true;
					}

					break;
				case L|R:
					if (keySt.state == PRESSED)
						start = WAIT_GetElapsedMillis(0);
					if (keySt.state == REPEATED && WAIT_GetElapsedMillis(start) >= PRESSING_TIME) {
						message = 1;
						toUpdateTime = false;
						menuRunning = true;
					}
					break;
				default:
					break;
			}
		}

	}
}

void vButtonHandler(void * pvParameters){
	key_state prev = {0, RELEASED};
	key_state keySt;
	int timeStamp = -1;
	while(1)  {
		keySt = BUTTON_GetButtonsEvents();
		if(keySt.key != prev.key || keySt.state != prev.state || uxQueueMessagesWaiting(xQueueButton) == 0) {
			if(keySt.state == REPEATED) {
				if(timeStamp == -1) {
					timeStamp = WAIT_GetElapsedMillis(0);
				} else if(WAIT_GetElapsedMillis(timeStamp) > LONG_PRESS_TIME) {//TODO CREATE CONSTANT
					timeStamp = -1;
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
	bool toInit = true;

	while(1) {
		if(gameRunning) {
			if(toInit) {
				GAME_Init();
				SPI_Init();
				while(ADXL345_Init() == -1);
				toInit = false;
			}

			GAME_Routine();

			gameRunning = false;
		}
	}
}

void vMenu(void * pvParameters){
	while(1) {
		if(menuRunning) {
			Menu();
			menuRunning = false;
		}
	}
}

void vLCDManager(void * pvParameters){
	LCDText_Init();
	bool cursor;

	LCD_Mesage message;
	while(1){
		if(xQueueReceive(xQueueLCD, &message, 200)) {

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

			if(message.cursor && !cursor) {
				cursor = true;
				LCDText_CursorOn();
			} else if(!message.cursor && cursor){
				LCDText_CursorOff();
				cursor = false;
			}
		}
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

	gameRunning = false;
	menuRunning = false;

	xQueueLCD = xQueueCreate(10,sizeof(LCD_Mesage));
	xQueueButton = xQueueCreate(20,sizeof(key_state));
	xQueueMqtt = xQueueCreate(3,sizeof(MQTT_Request));

	xTaskCreate(vConnection, "Connection", 600, (void *) 0, 1, NULL);

	xTaskCreate(vMainLoop, "MainLoop", 400, (void *) 0, 1, NULL);
	xTaskCreate(vButtonHandler, "ButtonHandler", 200, (void *) 0, 1, NULL);

	xTaskCreate(vGame, "Game", 200, (void *) 0, 1, NULL);
	xTaskCreate(vMenu, "Menu", 200, (void *) 0, 1, NULL);

	xTaskCreate(vLCDManager, "LCDManager", 200, (void *) 0, 1, NULL);
	xTaskCreate(vTimeHandler, "TimeHandler", 600, (void *) 0, 1, NULL);

	xTaskCreate(vMqttPublisher, "Mqtt", 700, (void *) 0, 1, NULL);

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
