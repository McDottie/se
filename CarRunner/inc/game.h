/*
 * game.h
 *
 *  Created on: 12 Jan 2021
 *      Author: Jose Filipe Cruz dos Santos
 */

#ifndef GAME_H_
#define GAME_H_

/**
 * @brief initializes the module
 */
void gameInit();

/**
 * @brief routine that interfaces with buttons, lcd and accelerometer to play the game. It also saves scores and player names.
 */
void gameRoutine(void);

#endif /* GAME_H_ */
