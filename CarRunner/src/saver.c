/*
 * saver.c
 *
 *  Created on: 15 Jan 2021
 *      Author: josee
 */
#include <stdlib.h>
#include "saver.h"
#include "flash.h"

int *names  = sector28;
int *scores = sector29;
int zero = 0;

void checkClean(){
	if(FLASH_BlanckCheck(28,29) != IAP_SECTOR_NOT_BLANK){
		FLASH_WriteData(names,&zero,512);
		FLASH_WriteData(scores,&zero,512);
	}
}

void saveScore(int score,char * name, int name_size) {
	checkClean();

	//names
	int currNameSize = names[0];

	int i = 0;
	int k = 1;

	int tmpNameHolder[currNameSize*17 + 1 + name_size + 1];

	for(;i < currNameSize;i++){
		int s = names[k];
		for (int j = 0; j <= s; j++) {
			tmpNameHolder[k] = names[k];
			k++;
		}
	}

	tmpNameHolder[0] = ++currNameSize;

	tmpNameHolder[k++] = name_size;
	for (int l = 0; l < name_size; l++) {
		tmpNameHolder[k++] = name[l];
	}

	if((FLASH_WriteArray(names,tmpNameHolder,k,512)) == IAP_CMD_SUCESS){

	}

	//scores
	int size = scores[0];
	i = 1;
	int tmpScoreHolder[size+2];

	for(; i <= size;i++){
		tmpScoreHolder[i] = scores[i];
	}
	tmpScoreHolder[0] = ++size;
	tmpScoreHolder[size] = score;

	int error;
	if((error=FLASH_WriteArray(scores,tmpScoreHolder,size+2,512)) == IAP_CMD_SUCESS){

	}
}


int* readScores() {
	checkClean();

	int size = scores[0];
	int * tmpScoreHolder = malloc(sizeof(int) * size);

	for(int i = 0; i < size;i++){
		tmpScoreHolder[i] = scores[i+1];
	}

	return tmpScoreHolder;
}

int listSize(){
	checkClean();

	return names[0];
}

void readNames(char * strings[]) {
	int k = 1;

	for (int i = 0; i < names[0]; i++) {
		int s = names[k];
		int j = 0;
		k++;
		strings[i] = malloc(17);
		for (; j < s; j++) {
			strings[i][j] = names[k];
			k++;
		}
		strings[i][j] = 0;
	}
}

