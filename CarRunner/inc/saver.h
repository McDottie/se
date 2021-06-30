/*
 * saver.h
 *
 *  Created on: 15 Jan 2021
 *      Author: Jose Filipe Cruz dos Santos
 */

#ifndef SAVER_H_
#define SAVER_H_

typedef struct {
	int score;
	int usernameLen;
	char * username;
} SAVER_Score;

void SAVER_SaveScore(int score,char * name, int name_size);

/**
 * @brief reads from flash all the scores
 * @return array of SAVER_Score's
 */
SAVER_Score ** SAVER_ReadScores();

/**
 * @return the current number of players that exist
 */
int SAVER_ListSize();


void SAVER_CleanScores(SAVER_Score ** scores,int size);

#endif /* SAVER_H_ */
