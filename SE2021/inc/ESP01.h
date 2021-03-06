/**
* @file		ESP01.h
* @brief	Contains the ESP01 API.
* @version	1.1
* @date		03 May 2021
* @author	Jose Filipe Cruz dos Santos
*
*/

#ifndef ESP01_H_
#define ESP01_H_

#include "uart.h"
#include <stdarg.h>
#include <stdbool.h>

#define ESP01_INSTRUCTION_PREFIX    "AT+"
#define ESP01_INSTRUCTION_SUFIX    	"\r\n"
#define ESP01_RESET					(5)
#define ESP01_BAUD_RATE             UART115200Hz
#define ESP01_MAX_RSP_SIZE			(1024)

enum ESP01_MODE {
	STATION = 1,
	SOFTAP,
	STATION_SOFTAP
};

enum ESP01_RECV_MODE{
	ACTIVE,
	PASSIVE
};

enum ESP01_Status {
	CONNECTED_AP = 2,
	CONNECTED_SERVER,
	DISCONNECTED_SERVER,
	DISCONNECTED_AP
};

enum ESP01_ConectionMode {
	SINGLE,
	MULTIPLE
};

void ESP01_Init();

bool ESP01_Test();

bool ESP01_VCommandPrefix(int timeout,char* prefix, char* command, unsigned char* response, char * start, unsigned char * format, va_list args);

bool ESP01_CommandPrefix(int timeout,char* prefix, char* command, unsigned char* response, char * start, unsigned char * format, ...);

bool ESP01_Command(int timeout, char* command, unsigned char* response, char * start, unsigned char * format, ...);

bool ESP01_SetMode(enum ESP01_MODE mode);

bool ESP01_VGetResponse(int timeout, char* response, char * start, unsigned char * format, va_list args);

bool ESP01_GetResponse(int timeout, char* response, char * start, unsigned char * format, ...);

int ESP01_ListAPs(char ** result);

bool ESP01_ConnectAP(char ssid[16], char password[32]);

bool ESP01_SetDNS(char* server0, char* server1);

bool ESP01_CIPDNS(bool enable,char* server0, char* server1);

bool ESP01_CIPMUX(enum ESP01_ConectionMode mode);

bool ESP01_ConnectServerId(char * connectionType, char * IP, int port, int id);

bool ESP01_ConnectServer(char * connectionType, char * IP, int port);

bool ESP01_ConnectServerKeepAliveId(char * connectionType, char * IP, int port, unsigned short int keepAlive, int id);

bool ESP01_ConnectServerKeepAlive(char * connectionType, char * IP, int port, unsigned short int keepAlive);

bool ESP01_SendId(char * message, int len, int id);

bool ESP01_Send(char * message, int len);

bool ESP01_Echo(bool echoOn);

bool ESP01_RecvMode(enum ESP01_RECV_MODE mode);

char * ESP01_RecvPassive();

int ESP01_RecvActiveId(char * retData, int count, int * id);

int ESP01_RecvActive(char * retData, int count);

enum ESP01_Status ESP01_CIPStatus();

#endif
