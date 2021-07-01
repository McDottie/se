/*
 * saver.c
 *
 *  Created on: 15 Jan 2021
 *      Author: josee
 */
#include "FreeRTOS.h"
#include "task.h"


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "saver.h"
#include "EEPROM.h"
#include "lcd.h"

#define MAX_NAME_SIZE 		(LCDText_COLUMNS+1)
#define TOP_SCORES_TO_SAVE 	(10)
#define SIZE 				((TOP_SCORES_TO_SAVE*MAX_NAME_SIZE) + (TOP_SCORES_TO_SAVE*2) + 1)
#define START_ADDRESS		(0)
int zero = 0;

SAVER_Score ** insertSorted(SAVER_Score ** scores, SAVER_Score * toAdd, int size){
	SAVER_Score ** newScores = pvPortMalloc(size+1);

	if(size == 0) {
		newScores[0] = toAdd;
		return newScores;
	}

	int j = 0;
	bool added;
	for(int i = 0; i < size; i++) {
		if(!added && scores[i]->score < toAdd->score) {
			newScores[j++] = toAdd;
			added = true;
		}
		newScores[j++] = scores[i];
	}

	if(!added) {
		newScores[j] = toAdd;
	}

	return newScores;
}

void SAVER_SaveScore(int score,char * name, int name_size) {
	int size = SAVER_ListSize();
	SAVER_Score ** scores = SAVER_ReadScores();
	SAVER_Score scoreToAdd = {score,name_size,name};

	if(size >= TOP_SCORES_TO_SAVE) {
		if(scores[size-1]->score < score) {
			size--;
		} else {
			SAVER_CleanScores(scores, size);
			return;
		}
	}

	SAVER_Score ** sortedScores = insertSorted(scores,&scoreToAdd, size);
	uint8_t bytes[SIZE];
	int totalSize = 0;
	bytes[totalSize++] = size+1;

	for (int i = 0; i < size+1; i++) {
		bytes[totalSize++] = sortedScores[i]->score;
		bytes[totalSize++] = sortedScores[i]->usernameLen;
		for(int j = 0; j < sortedScores[i]->usernameLen; j++) {
			bytes[totalSize++] = sortedScores[i]->username[j];
		}
	}

	SAVER_CleanScores(scores, size);
	vPortFree(sortedScores);

	EEPROM_WriteData(START_ADDRESS, bytes
			, totalSize);
}

int SAVER_ListSize(){
	uint8_t size;
	EEPROM_ReadByte(0, &size);
	return (int)size;
}

SAVER_Score ** SAVER_ReadScores() {
	uint8_t bytes[SIZE];

	EEPROM_ReadData(0, bytes, SIZE);
	SAVER_Score ** scores = pvPortMalloc(bytes[0]);

	int namesSizeSum = 0;
	int i = 0;
	int j = 0;
	for(int count = 0; count < bytes[0]; count++) {
        int size = bytes[i+2];
		namesSizeSum += size;
		int end = size + i+3;

		SAVER_Score * currScore = pvPortMalloc(sizeof(SAVER_Score));
		currScore->score = bytes[i+1];
		currScore->usernameLen = size;
		currScore->username = pvPortMalloc(size);
		int k = 0;
		for(int l = i+3; l < end; l++) {
			currScore->username[k++] = bytes[l];
		}
		currScore->username[k] = 0;
		scores[j++] = currScore;
		i = namesSizeSum + (count+1)*2;
	}
	return scores;
}

void SAVER_CleanScores(SAVER_Score ** scores,int size){
	for(int i = 0; i < size; i++) {
		vPortFree(scores[i]->username);
		vPortFree(scores[i]);
	}
	vPortFree(scores);
}
