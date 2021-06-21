/*
 * timeHelper.c
 *
 *  Created on: 5 Jan 2021
 *      Author: $(author)
 */
#include "lcd.h"
#include "time_helper.h"
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
void Time_changeRoutine(time_t seconds) {

    LCDText_Clear();
    struct tm *dateTime = gmtime(&seconds);

    update_DateTimeDisplay(*dateTime);
    LCDText_Locate(1, 0);
	LCDText_WriteString("Maintenance Mode");


    key_state state = BUTTON_GetButtonsEvents();
    int * to_sum[] = {&dateTime->tm_min,&dateTime->tm_hour,
    			      &dateTime->tm_mday,&dateTime->tm_mon,&dateTime->tm_year};
    int count = 0;
    int pos[5] = {15,12,9,6,3};
	LCDText_CursorOn();
    LCDText_Locate(0, pos[count]);

    while(1) {
    	if(state.state == PRESSED) {
    		switch(state.key) {
				case 1:
					*to_sum[count] += 1;
					update_DateTimeDisplay(*dateTime);
					LCDText_Locate(0, pos[count]);
					break;
				case 2:
					*to_sum[count] -= 1;
					if(*to_sum[count] < 0)
						*to_sum[count] += 1;
					update_DateTimeDisplay(*dateTime);
					LCDText_Locate(0, pos[count]);
					break;
				case 4:
					count++;
					if(count >= 5) {
						RTC_SetValue(dateTime);
						LCDText_CursorOff();
						LCDText_Locate(1, 0);
						LCDText_WriteString("                ");
						return;
					}
					LCDText_Locate(0, pos[count]);
					break;
				default:
					break;
			}
    	}

        state = BUTTON_GetButtonsEvents();
    }
    LCDText_CursorOff();
}


void update_DateTimeDisplay(struct tm dateTime){
	LCDText_Locate(0,0);
	char buffer[26];
	strftime(buffer, 26, "%Y-%m-%d %H:%M", &dateTime);
	LCDText_WriteString(buffer);
}