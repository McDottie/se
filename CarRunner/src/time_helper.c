/*
 * timeHelper.c
 *
 *  Created on: 5 Jan 2021
 *      Author: $(author)
 */
#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#include "time_helper.h"
#include "transport.h"

#include "definitions_variables.h"
#include "lcd.h"
#include "NTP.h"
#include "button.h"
#include "time.h"


/**
 @startuml
	[*] -> Time_changeRoutine
	title CarRunner Change Date Time Routine
	Time_changeRoutine --> AddTime : keypressed == L
	AddTime: value++
	Time_changeRoutine --> SubtractTime : keypressed == R
	SubtractTime --> Time_changeRoutine
	SubtractTime : value--
	AddTime --> Time_changeRoutine
	Time_changeRoutine --> ChangeTimeUnitOrExit : keypressed = S
	ChangeTimeUnitOrExit : timeUnit++
	ChangeTimeUnitOrExit --> Time_changeRoutine
	ChangeTimeUnitOrExit --> [*]

@enduml
*/
void TIME_ChangeRoutine(time_t seconds) {

    struct tm *dateTime = gmtime(&seconds);

    char * maintenance = pvPortMalloc(17);
    strcpy(maintenance,"Maintenance Mode");

    printComplex(10, true, true, true, false, 1, 0, maintenance);

    int * to_sum[] = {&dateTime->tm_min,&dateTime->tm_hour,
    			      &dateTime->tm_mday,&dateTime->tm_mon,&dateTime->tm_year};
    int count = 0;
    int pos[5] = {15,12,9,6,3};
    printComplex(10, false, true, false, true, 0, pos[count], "");

	key_state keySt;

	TIME_UpdateDateTimeDisplay(*dateTime);

	while(1) {

		if(!xQueueReceive(xQueueButton, &keySt, 100)) {
			continue;
		}

    	if(keySt.state == PRESSED) {
    		switch(keySt.key) {
				case 1:
					*to_sum[count] += 1;
					TIME_UpdateDateTimeDisplay(*dateTime);
					printLocate(10, true, 0, pos[count]);
					break;
				case 2:
					*to_sum[count] -= 1;
					if(*to_sum[count] < 0)
						*to_sum[count] += 1;
					TIME_UpdateDateTimeDisplay(*dateTime);
					printLocate(10,true, 0, pos[count]);
					break;
				case 4:
					count++;
					if(count >= 5) {
						RTC_SetValue(dateTime);
						char * string = pvPortMalloc(17);
						strcpy(string,"                ");
						printComplex(10, true, true, true, false, 1, 0,string);
						return;
					}
					printLocate(10, true, 0, pos[count]);
					break;
				default:
					break;
			}
    	}

    }
    printComplex(10, false, false, false, false, 0, 0, "");
}

void TIME_UpdateDateTimeDisplay(struct tm dateTime){
	char * buffer = pvPortMalloc(26);
	//strftime(buffer, 26, "%Y-%m-%d %H:%M", &dateTime);
	sprintf(buffer, "%04d/%02d/%02d %02d:%02d",dateTime.tm_year+1900,dateTime.tm_mon+1, dateTime.tm_mday, dateTime.tm_hour,dateTime.tm_min);
	printComplex(100, false, true, true, false, 0, 0, buffer);
}

int bindIdToSendTime(unsigned char *address, unsigned int bytes) {
	return NETWORK_Send(address,bytes,TIME_CONNECTION_ID);
}

int bindIdToRecvTime(unsigned char *address, unsigned int bytes) {
	return NETWORK_Recv(address, bytes, TIME_CONNECTION_ID);
}

int bindIdToConnectTime(unsigned char * type, unsigned char *server, unsigned int port) {
	return NETWORK_Connect(type, server, port, 10, TIME_CONNECTION_ID);
}


void vTimeHandler(){

	while(conStatus != CON) vTaskDelay(100);

	static transport_iofunctions_t iof = { bindIdToSendTime, bindIdToRecvTime, bindIdToConnectTime};

	if(!NTP_Init(NTP_DEFAULT_SERVER, NTP_DEFAULT_PORT,iof)) {
		RTC_Init(0);
	} else {
		RTC_Init(NTP_Request(5));
	}

	int dateTime_Stamp = WAIT_GetElapsedMillis(0);
	struct tm dateTime;

	while(1) {
		if(toUpdateTime && WAIT_GetElapsedMillis(dateTime_Stamp) >= 1000) {
			dateTime_Stamp = WAIT_GetElapsedMillis(0);
			RTC_GetValue(&dateTime);
			TIME_UpdateDateTimeDisplay(dateTime);
		}
	}
}
