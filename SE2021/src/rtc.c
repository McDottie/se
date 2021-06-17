/*
 * rtc.c
 *
 *  Created on: 3 Dez 2020
 *      Author: José Santos
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "rtc.h"

void RTC_Init(time_t seconds) {
    //LPC_SC->PCONP |= (1 << 9); //TURN ON RTC

    LPC_RTC->CCR = 0b10; // DISABLE AND RESET

    LPC_RTC->CIIR = 0x0;
    LPC_RTC->CCR = 0b01; // ENABLE AND REMOVE RESET
    
    RTC_SetSeconds(seconds);
}

void RTC_GetValue(struct tm *dateTime) {
    long s_m_h_dow = LPC_RTC->CTIME0;
    long dom_m_y = LPC_RTC->CTIME1;
    long doyear = LPC_RTC->CTIME1;

    dateTime->tm_sec = s_m_h_dow & 0x3f;
    dateTime->tm_min = (s_m_h_dow >> 8) & 0x3f;
    dateTime->tm_hour = (s_m_h_dow >> 16) & 0x1f;
    dateTime->tm_wday = (s_m_h_dow >> 24);

    dateTime->tm_mday = dom_m_y & 0xf;
    dateTime->tm_mon = ( (dom_m_y >> 8) & 0xf ) - 1;
    dateTime->tm_year = ( (dom_m_y >> 16) & 0xfff ) - 1900;


    dateTime->tm_yday = (doyear >> 16) & 0xff;
    
}


void RTC_SetValue(struct tm *dateTime) {

    LPC_RTC->SEC = dateTime->tm_sec;
    LPC_RTC->MIN = dateTime->tm_min;
    LPC_RTC->HOUR = dateTime->tm_hour;
    LPC_RTC->DOM = dateTime->tm_mday;
    LPC_RTC->DOW = dateTime->tm_wday;
    LPC_RTC->DOY = dateTime->tm_yday;
    LPC_RTC->MONTH = dateTime->tm_mon + 1;
    LPC_RTC->YEAR = dateTime->tm_year + 1900;

}


void RTC_SetSeconds(time_t seconds) {
	RTC_SetValue(gmtime(&seconds));
}


time_t RTC_GetSeconds(void) {
	struct tm dateTime;
	RTC_GetValue(&dateTime);
	return mktime(&dateTime);
}
