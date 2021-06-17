/*
 * NTP.c
 *
 *  Created on: 5 May 2021
 *      Author: josee
 */
#include "NTP.h"

int NTP_Init(char* server, int port) {
    return ESP01_ConnectServer("UDP", server, port);
}

uint8_t * NTP_MessageFromPacket(ntp_packet packet){

}

ntp_packet * NTP_PacketFromMessage(uint8_t * packet){

}

ntp_packet NTP_Request(uint32_t lastCorrectTime, uint32_t currentTime) {
    ntp_packet defaultSendPacket = {
        .li_vn_mode = li_vn_mode,
        .stratum = stratum,
        .poll = poll,
        .precision = precision,
        .refTm_s = lastCorrectTime,
        .origTm_s = currentTime,
    };

    //TODO ESP01_Send(char * message, int len);

    return defaultSendPacket;
}


