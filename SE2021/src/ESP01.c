/*
 * ESP01.c
 *
 *  Created on: 3 May 2021
 *      Author: José Santos
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

//#include <sys/types.h> // should not be needed
//#include <regex.h>
#include "FreeRTOS.h"

#include <string.h>
#include <stdio.h>
#include "printf-stdarg.c"
#include <stdarg.h>
#include <stdbool.h>
#include "ESP01.h"
#include "utils.h"
#include "wait.h"

void ESP01_Init() {
    UTILS_SetPinDir(GPIO0, ESP01_RESET, OUTPUT);

    LPC_GPIO0->FIOSET = (1<<ESP01_RESET);

    WAIT_Milliseconds(1);

    LPC_GPIO0->FIOCLR = (1<<ESP01_RESET);

    WAIT_Milliseconds(100); //prevent cleaning

    LPC_GPIO0->FIOSET = (1<<ESP01_RESET);

    WAIT_Milliseconds(2000); //prevent cleaning

    UART_Initialize(ESP01_BAUD_RATE, UART2);

    //clean
    unsigned char tmp[100];
    int prev = 1;
    int prevC = 0;
	/*while(!(prev == 0 && prevC == 10)){
		if(prev == 0) prevC++;

		prev = UART_ReadBuffer(&tmp, 100);
		WAIT_Milliseconds(1);
	}*/

}

bool ESP01_Restore(){
	char result[4];
	return ESP01_Command(10000, "RESTORE", "O", result, "OK");
}

bool ESP01_Test() {
    UART_Printf("%s%s","AT",ESP01_INSTRUCTION_SUFIX);
    unsigned char c[3];
    int res;

    return ESP01_GetResponse(100000,c,"O","OK");;
}

int ESP01_NumberOfArgs(char* format){
	int count = 0;
	int pos = 0;

	while(true){
		char * tmp = strchr(&format[pos], '%');
		if(tmp == 0) {
			return count;
		}
		if(tmp-format == 0) tmp++;

		pos = tmp - format + 1;
		count++;
	}
}

bool ESP01_GetResponse(int timeout, char* response, char * start, unsigned char * format, ...) {
	va_list args;
	va_start(args, format);
	return ESP01_VGetResponse(timeout, response, start, format,args);
}

bool ESP01_VGetResponse(int timeout, char* response, char * start, unsigned char * format, va_list args) {
	int count = ESP01_NumberOfArgs(format);
	int startLen = strlen(start);
	//regex_t * regex;
	//regcomp(regex, format, REG_NOSUB);
	bool started = false;
	int sum = 0;
	bool read = true;
	int startT = WAIT_GetElapsedMillis(0);
	char * responseHolder = pvPortMalloc(ESP01_MAX_RSP_SIZE);
	memset(responseHolder, 0, ESP01_MAX_RSP_SIZE);

	vTaskDelay(1000);
	while(WAIT_GetElapsedMillis(startT) < timeout){
		read = UART_GetChar(&responseHolder[sum]);
		if(read) {
			sum++;
			int len = strlen(responseHolder);
			if(!started && len == startLen) {
				if(strcmp(responseHolder,start) != 0) {
					memset(responseHolder, 0, ESP01_MAX_RSP_SIZE);
					sum = 0;
				} else {
					started = true;
				}
			}

			responseHolder[sum] = 0;
			int res = vsscanf(responseHolder,format, args);

			if(res == 0 && strcmp(responseHolder,format) == 0) {
				//int i = regexec(regex, responseHolder,0 , NULL, REG_NOTBOL);

				memcpy(response, responseHolder, sum);
				vPortFree(responseHolder);
				return true;
			} else if(res == count && count != 0) {
				memcpy(response, responseHolder, sum);

				vPortFree(responseHolder);
				return true;
			}
		}
	}


	vPortFree(responseHolder);
	return false;
}

bool ESP01_CommandPrefix(int timeout,char* prefix, char* command, unsigned char* response, char * start, unsigned char * format, ...){
	va_list args;
	va_start(args, format);
	return ESP01_VCommandPrefix(timeout,prefix,command,response,start,format, args);
}

bool ESP01_VCommandPrefix(int timeout,char* prefix, char* command, unsigned char* response, char * start, unsigned char * format, va_list args){
	UART_Printf("%s%s%s",prefix,command,ESP01_INSTRUCTION_SUFIX);

	bool res = ESP01_VGetResponse(timeout,response,start,format,args);
	return res;
}

bool ESP01_Command(int timeout, char* command, unsigned char* response, char * start, unsigned char * format, ...) {
	va_list args;
	va_start(args, format);
	bool ret = ESP01_VCommandPrefix(timeout,ESP01_INSTRUCTION_PREFIX, command, response, start, format,args);
	return ret;
}

bool ESP01_SetMode(enum ESP01_MODE mode) {
	char cmd[9];
	strcpy(cmd,"CWMODE=");
	cmd[7] = '0' + mode;
	cmd[8] = 0;
    char * response = pvPortMalloc(4);

    return ESP01_Command(3000, cmd, response, "O", "OK");
}


int ESP01_ListAPs(char ** result){
	char * aps = pvPortMalloc(550);
	char * res = pvPortMalloc(550);
	char ok[3];
	// regex "\+CWLAP:.*\\nOK"

	ESP01_Command(10000, "CWLAP", res, "+CWLAP:", "+CWLAP:%s%[^OK]", aps,ok);
	vPortFree(aps);
	//TODO
	return 0;
}

bool ESP01_ConnectAP(char ssid[16], char password[32]){
	int ssidS = strlen(ssid);
	int passS = strlen(password);
	char cmd[12+ssidS+passS];

	sprintf(cmd, "CWJAP=\"%s\",\"%s\"",ssid,password);

	char * response = pvPortMalloc(4);
	bool r = ESP01_Command(30000, cmd, response, "O","OK");
	return r;
}

bool ESP01_SetDNS(char* server0, char* server1){
	return ESP01_CIPDNS(true, server0, server1);
}

bool ESP01_CIPDNS(bool enable,char* server0, char* server1){
	char cmd[33];

	if(strlen(server0) != 7 && strlen(server1) != 7)
		return false;

	sprintf(cmd, "CIPDNS_CUR=%d,\"%s\",\"%s\"",enable,server0,server1);
	char response[3];
	bool r =  ESP01_Command(30000, cmd, response, "O", "OK");
	return r;
}

bool ESP01_ConnectServer(char * connectionType, char * IP, int port){
	int len = strlen(IP) + strlen(connectionType) + 20;
	char cmd[len];
	sprintf(cmd, "CIPSTART=\"%s\",\"%s\",%d",connectionType, IP, port);
	char response[3];
	bool r = ESP01_Command(30000, cmd, response, "O", "OK");
	return r;
}

bool ESP01_Send(char * message, int len){

	char cmd[21];

	sprintf(cmd, "CIPSEND=%d",len);

	char response[8];
	if(ESP01_Command(30000, cmd, response, "O", ">")) {
		UART_WriteBuffer(message, len);

	} else {
		return false;
	}

	bool r =  ESP01_GetResponse(3000, response,"O", "SEND OK");
	return r;
}

bool ESP01_Echo(bool echoOn) {
	char cmd[3];
	sprintf(cmd, "E%d",echoOn);
	char result[4];
	return ESP01_CommandPrefix(10000,"AT", cmd, result, "O", "OK");
}

bool ESP01_RecvMode(enum ESP01_RECV_MODE mode){
	char cmd[14];
	strcpy(cmd,"CIPRECVMODE=");
	cmd[12] = '0' + mode;
	cmd[13] = 0;
    char * response = pvPortMalloc(4);

    ESP01_Command(10000, cmd, response, "O", "OK");
}

char * ESP01_RecvPassive(){
	char * response = pvPortMalloc(17);
	int len = 0;
	ESP01_Command(10000, "CIPRECVLEN", response,"+CIPRECVLEN:", "+CIPRECVLEN:%d", len);

	vPortFree(response);

	char cmd[17];
	sprintf(cmd, "CIPRECVDATA=%d",len);

	response = pvPortMalloc(len+7);
	char * data = pvPortMalloc(len+1);

	int actualLen;

	ESP01_Command(10000, "CIPRECVDATA=", response,"+CIPRECVDATA:", "+CIPRECVDATA:(%d),(%s)%[^OK]",data,actualLen);
	vPortFree(response);
	return data;
}

char * ESP01_RecvActive() {
	char response[10];
	int len = 0;
	ESP01_GetResponse(10000, response, "+IPD", "+IPD,%d[^:]", len);
	len +=1;
	return "H";
}