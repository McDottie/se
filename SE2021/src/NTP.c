/*
 * NTP.c
 *
 *  Created on: 5 May 2021
 *      Author: josee
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "NTP.h"
#include "ESP01.h"

uint8_t li_vn_mode = 0xE3; // LI, Version, Mode
uint8_t stratum = 0; // Stratum, or type of clock
uint8_t poll = 6; // Polling Interval
uint8_t precision = 0xEC; // Peer Clock Precision

uint32_t txTm_s; // Transmit time-stamp seconds
uint32_t txTm_f; // Transmit time-stamp fraction

uint32_t ntohl(uint32_t netlong) {
	return __builtin_bswap32(netlong);
}


int NTP_Init(char* server, int port) {
    return ESP01_ConnectServer("UDP", server, port);
}

ntp_packet NTP_Request() {//uint32_t lastCorrectTime, uint32_t currentTime) {
    ntp_packet defaultSendPacket = {
        .li_vn_mode = li_vn_mode,
        .stratum = stratum,
        .poll = poll,
        .precision = precision,
        //.refTm_s = lastCorrectTime,
        //.origTm_s = currentTime,
    };

    ESP01_Send((char *)&defaultSendPacket, 48);
    char * result = ESP01_RecvActive();
    ntp_packet * retPacket = (ntp_packet *)result;
    return defaultSendPacket;
}




