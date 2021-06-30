/*
 * mqtttask.h
 *
 *  Created on: 27/06/2021
 *      Author: josee
 */

#ifndef MQTT_H_
#define MQTT_H_

#define CONNECTION_KEEPALIVE_S 	60UL

#define MQTT_ADDRESS			"iot-se2021.ddns.net"
#define MQTT_PORT				1883
#define MQTT_DEVICE_TOKEN		"SE2-BEST-RECORDS"


typedef struct mqtt_request{
	int len;
	char * payload;
	char * topicName;
} MQTT_Request;

enum state {
	STATE_INIT,
	STATE_CONNECT,
	STATE_WAIT_CONNECT,
	STATE_PUBLISH
};

void vMqttPublisher(void *pvParameters);

#endif /* MQTT_H_ */
