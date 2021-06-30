/*
 * time_helper.h
 *
 *  Created on: 5 Jan 2021
 *      Author: Jose Filipe Cruz dos Santos
 */

#ifndef TIME_HELPER_H_
#define TIME_HELPER_H_

#include "rtc.h"

/**
 * @brief routine that interfaces with lcd and buttons to change time in rtc
 * @param seconds starting time
 * */
void TIME_ChangeRoutine(time_t seconds);

/**
 * @brief updates lcd with date and time
 * @param datetime time to be displayed
 * */
void TIME_UpdateDateTimeDisplay(struct tm datetime);

void vTimeHandler();

#endif /* TIME_HELPER_H_ */
