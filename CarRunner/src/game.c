/*
 * game.c
 *
 *  Created on: 12 Jan 2021
 *      Author: $(author)
 */

#include <definitions_variables.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "saver.h"
#include "wait.h"
#include "lcd.h"
#include "button.h"
#include "ADXL345.h"
#include "CarRunner.h"
#include "publisher.h"

#define CAR 1
#define OBS 2



bool car = 0;
bool upRoad[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool downRoad[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int probabilityToSpawn = 20;
int points = 0;
int delay = 1000;

void GAME_Init(){
	unsigned char car[] = {0x00, 0x00, 0x00, 0x00, 0x0C, 0x1F, 0x0A, 0x00};
	unsigned char obs1[] = {0x00, 0x04, 0x0A, 0x15, 0x0A, 0x04, 0x00, 0x00};

	LCDText_CreateChar(1, car);
	LCDText_CreateChar(2, obs1);
}

void initValues() {
	points = 0;
	delay = 1000;
	probabilityToSpawn = 20;
	car = 0;
	for(int i = 0; i < 16; i++) {
		upRoad[i] = 0;
		downRoad[i] = 0;
	}
}

int updateGameLogic(){
    //move blocks up the roads and create new blocks
    int random1 = rand() % 100;
    int new1 = random1 < probabilityToSpawn/2 ? 1 : 0;
    for(int i=0;i<16;i++)
    {
    	upRoad[i] = upRoad[i+1];
    }


    int random2 = rand() % 100;
    int new2 = random2 < probabilityToSpawn/2 ? 1 : 0;
    for(int i=0;i<16;i++)
    {
    	downRoad[i] = downRoad[i+1];
    }

    //make possible to pass trough
    if(new1 && new2) {
    	new1 = 0;
    }

    if(new1 && downRoad[14]) {
    	new1 = 0;
    }

    if(new2 && upRoad[14]) {
        new2 = 0;
    }

    upRoad[15]=new1;
    downRoad[15]=new2;

    //checkGameOver
    if(upRoad[0]) {
        if (!car) {
            return 1;
        }
        points++;
        probabilityToSpawn += 5;
        delay -= 50;
    }

    if(downRoad[0]) {
        if (car) {
            return 1;
        }
        points++;
        probabilityToSpawn += 5;
        delay -= 50;
    }

    if (delay < 100)
    	delay = 100;
    if (probabilityToSpawn > 30)
    	probabilityToSpawn = 30;

    return 0;

}

void updateGameDesign() {
    char up[17];
    char down[17];


    for(int i=0;i<16;i++)
    {
        if(upRoad[i]) {
			up[i] = OBS;
        }
        else
        	up[i] = ' ';

        if(downRoad[i]) {
        	down[i] = OBS;
        }
        else
        	down[i] = ' ';
    }

    if(!car) {
		up[0] = CAR;
	} else {
		down[0] = CAR;
	}
    up[16] = 0;
    down[16] = 0;

    char * upP = pvPortMalloc(17);
    char * downP = pvPortMalloc(17);
    strcpy(upP,up);
    strcpy(downP,down);

    printComplex(100, false, true, true, false, 0, 0, upP);
    printComplex(100, false, true, true, false, 1, 0, downP);

}


/**
 @startuml
	[*] -> saveUser
	title CarRunner Save User
	saveUser --> NextChar : keypressed == L
	saveUser --> PreviousChar : keypressed == R
	NextChar : char++
	PreviousChar : char--
	saveUser --> AddChar : keypressed == S
	AddChar : name[i] = char
	AddChar : i++
	saveUser --> save : keypressed == S for 2 seconds
	save --> [*]
@enduml
*/
char * saveUser(int points){
	char * name = pvPortMalloc(17);

	char * string = pvPortMalloc(17);
	strcpy(string,"INSERT YOUR NAME");
    printSimple(100, string);

	int pos = 0;
	char currChar = 64;

	int stamp = WAIT_GetElapsedMillis(0);
	printComplex(100, false, true, false, true, 1, 0, "");


	key_state keySt = {0, RELEASED};

	while(1) {

		if(!xQueueReceive(xQueueButton, &keySt, 100)) {
			keySt.key = 0;
			keySt.state = RELEASED;
		}

    	if (keySt.state == REPEATED && keySt.key == S && WAIT_GetElapsedMillis(stamp) > 2000) {
    		break;
    	}

		if (keySt.state == PRESSED) {
			stamp = WAIT_GetElapsedMillis(0);

			switch(keySt.key){
				case L:
					currChar++;
					printComplexChar(100, false, false, true, true, 0, 0, currChar);
					printLocate(100, true, 1, pos);
					break;
				case R:
					currChar--;
					printComplexChar(100, false, false, true, true, 0, 0, currChar);
					printLocate(100, true, 1, pos);
					break;
				case S:
					name[pos] = currChar;
					printLocate(100, true, 1, ++pos);
					currChar = 64;
					break;
				default:
					break;
			}
		}

    }
	printComplex(100, false, false, false, false, 0, 0, "");

	name[pos] = 0;

	SAVER_SaveScore(points,name,pos);
	return name;
}

/**
 @startuml
	[*] -> gameRoutine
	title CarRunner Game Routine
	gameRoutine --> updateGameDesign : after delay s
	gameRoutine --> updateGameLogic : after delay s
	gameRoutine --> updateGameDesign : keypressed == L | R or tap detected
	gameRoutine --> updateGameLogic : keypressed == L | R or tap detected
	updateGameLogic --> gameRoutine
	updateGameDesign --> gameRoutine
	updateGameLogic : update_roads
	updateGameLogic : check_gameOver
	gameRoutine --> saveUser : gameOver
	saveUser --> [*]
@enduml
*/
void GAME_Routine(){
	srand(WAIT_GetElapsedMillis(0));

	initValues();

    int elapsed = WAIT_GetElapsedMillis(0) + 1000;
    key_state keySt = {0, RELEASED};

    printComplex(10, true, false, false, false, 0, 0, "");

    bool gameOver = false;
    do {
    	bool tap = ADXL345_isTap();

    	if(tap) {
    		if(car)
    			car = 0;
    		else
    			car = 1;
    		updateGameDesign();
    	}

		if(!xQueueReceive(xQueueButton, &keySt, 100)) {
			keySt.key = 0;
			keySt.state = RELEASED;
		}

    	if (keySt.state == PRESSED) {
    		switch(keySt.key){
				case L:
					car = 0;
					break;
				case R:
					car = 1;
					break;
    		}
    		updateGameDesign();
    	}

        if(WAIT_GetElapsedMillis(elapsed) >= delay) {
            gameOver = updateGameLogic();
            updateGameDesign();
            elapsed = WAIT_GetElapsedMillis(0);
        }

    }while(!gameOver);

    //GAME Over screen

    char * string = pvPortMalloc(13);
	strcpy(string,"  GAME OVER!");
	printSimple(100, string);

	printfComplex(100, false, true, true, false, 1, 0, "POINTS: %d",points);

    WAIT_Milliseconds(3000);

    char * username = saveUser(points);

    PUBLISHER_Publish(GROUP_NR, points, username);

    vPortFree(username);
    printComplex(100, true, false, false, false, 0, 0, "");

}
