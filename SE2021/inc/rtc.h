/**
* @file		rtc.h
* @brief	Contains the rtc peripheral manager API.
* @version	1.1
* @date		3 Nov 2020
* @author	Jose Filipe Cruz dos Santos
*
* */

#ifndef RTC_H_
#define RTC_H_

#include "time.h"

/** @brief Initiates the system to allow access to the RTC peripheral.
 * @param seconds  The RTC is started with this value, which represents the seconds since 00:00:00 UTC from 1 January 1970.
 */
void RTC_Init(time_t seconds);

/** 
 *  @param[out]  dateTime current RTC value ajusted to 00:00:00 UTC from 1 January 1970.
*/
void RTC_GetValue(struct tm *dateTime);

/**
 * @brief Updates RTC
 * @param dateTime value that is set to update RTC. 
 **/
void RTC_SetValue(struct tm *dateTime);

/**
 * @brief Performs the RTC update with the value of the seconds parameter
 * @param seconds represents the seconds since 00:00:00 UTC of 1 January 1970.
 */
void RTC_SetSeconds(time_t seconds);

/**
 * @return Returns the current value of the RTC, in seconds since 00:00:00 UTC on January 1 1970
 * */
time_t RTC_GetSeconds(void);

#endif /* RTC_H_ */
