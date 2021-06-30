/*
 * mqtttask.c
 *
 *  Created on: 27/06/2021
 *      Author: josee
 */

#include <mqtt.h>
#include "MQTTPacket.h"
#include "network.h"
#include "transport.h"
#include "definitions_variables.h"

void MQTT_Publish(char * topic, char * payload, int size){
	MQTT_Request req = {size,payload,topic};
	xQueueSend(xQueueMqtt, &req, 100);
}

int bindIdToSend(unsigned char *address, unsigned int bytes) {
	return NETWORK_Send(address,bytes,MQTT_CONNECTION_ID);
}

int bindIdToRecv(unsigned char *address, unsigned int bytes) {
	int id;
	return NETWORK_Recv(address, bytes, &id);
}

void vMqttPublisher(void *pvParameters) {
	unsigned char buffer[128];
	MQTTTransport transporter;
	int result;
	int length;

	MQTT_Request req;
	while(xQueueReceive(xQueueMqtt, &req, 200));

	while (conStatus != CON) {
		vTaskDelay(100);
	}

	// ESP8266 Transport Layer
	static transport_iofunctions_t iof = { bindIdToSend, bindIdToRecv };
	int transport_socket = transport_open(&iof);

	int state = STATE_INIT;
	while (1) {

		switch (state) {
			case STATE_INIT:
				if (NETWORK_Connect("TCP", MQTT_ADDRESS, MQTT_PORT, CONNECTION_KEEPALIVE_S,MQTT_CONNECTION_ID)) {
					state++;
				}
				break;
			case STATE_CONNECT:
				transporter.sck = &transport_socket;
				transporter.getfn = transport_getdatanb;
				transporter.state = 0;

				MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
				connectData.MQTTVersion = 3;
				connectData.username.cstring = MQTT_DEVICE_TOKEN;
				connectData.clientID.cstring = MQTT_DEVICE_TOKEN;
				connectData.keepAliveInterval = CONNECTION_KEEPALIVE_S * 2;
				length = MQTTSerialize_connect(buffer, sizeof(buffer), &connectData);
				// Send CONNECT to the mqtt broker.
				if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
					state++;
				} else {
					state = STATE_INIT;
				}
				break;
			case STATE_WAIT_CONNECT:
				// Wait for CONNACK response from the MQTT broker.
				while (true) {
					if ((result = MQTTPacket_readnb(buffer, sizeof(buffer), &transporter)) == CONNACK) {
						// Check if the connection was accepted.
						unsigned char sessionPresent, connack_rc;
						if ((MQTTDeserialize_connack(&sessionPresent, &connack_rc, buffer,
								sizeof(buffer)) != 1) || (connack_rc != 0)) {
							state = STATE_INIT;
							break;
						} else {
							state++;
							break;
						}
					} else if (result == -1) {
						state = STATE_INIT;
						break;
					}
				}
				break;
			case STATE_PUBLISH:;
				MQTTString topicString = MQTTString_initializer;
				topicString.cstring = req.topicName;

				unsigned char * payload = req.payload;

				length = req.len;//sprintf(payload, "{\"group\":%d, \"score\":%d}", groupId, score);
				length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, 0, 0, 0, topicString, payload, length);
				// Send PUBLISH to the MQTT broker.
				if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length) {
					vPortFree(req.topicName);
					vPortFree(req.payload);
					// Wait for new data.
					while(xQueueReceive(xQueueMqtt, &req, 200));
				} else {
					state = STATE_INIT;
				}
				break;
			default:
				state = STATE_INIT;
		}
	}
}
