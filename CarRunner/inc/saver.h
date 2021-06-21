/*
 * saver.h
 *
 *  Created on: 15 Jan 2021
 *      Author: Jose Filipe Cruz dos Santos
 */

#ifndef SAVER_H_
#define SAVER_H_

void saveScore(int score,char * name, int name_size);

/**
 * @brief reads from flash all the scores
 * @return array of scores
 */
int* readScores();

/**
 * @return the current number of players that exist
 */
int listSize();

/**
 * @brief function that reads from flash all the names its required that strings is started with size[listSize][17] to accommodate for max size names
 * */
void readNames(char* strings[]);

#endif /* SAVER_H_ */
