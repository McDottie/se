/*
 * publisher.c
 *
 *  Created on: 28/06/2021
 *      Author: $(author)
 */


#include "publisher.h"

void PUBLISHER_Publish(int group, int score, char * username){
	char * topic = pvPortMalloc(24);
	strcpy(topic,"v1/devices/me/telemetry");
	char * payload = pvPortMalloc(25);
	int len = sprintf(payload, "{\"group\":%d, \"score\":%d, \"username\":%s}", group, score, username);

	MQTT_Publish(topic, payload, len);
}
