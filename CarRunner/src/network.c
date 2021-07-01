/*
 * network.c
 *
 *  Created on: 25/06/2021
 *      Author: $(author)
 */


#include "ESP01.h"
#include "network.h"
#include "definitions_variables.h"

QueueHandle_t xQueueSend,xQueueRecv;

bool NETWORK_Init(void) {
	if(!ESP01_SetMode(STATION))return false;
	//if(!ESP01_ConnectAP("LAPTOP-TQA8JN5P 2398", "S%3144b4")) return false;
	//if(!ESP01_ConnectAP("HUAWEI", "12345678")) return false;
	if(!ESP01_ConnectAP("Vodafone-3A8807", "V9g2ZEsVM7")) return false;
	ESP01_SetDNS("1.1.1.1","1.0.0.1");
	ESP01_CIPMUX(MULTIPLE);

	return true;
}

bool NETWORK_Connect(const char * type, const char *host, const unsigned short int port, const unsigned short int keepalive, int id) {
	return ESP01_ConnectServerKeepAliveId(type,host,port,keepalive,id);
}

int NETWORK_Send(unsigned char *address, unsigned int bytes, int id) {
	/*
	 *
	ESP_Message message  = {message,bytes,id};

	return xQueueSend(xQueueSend, &message, portMAX_DELAY) ? bytes : -1;
	}
*/
	if(ESP01_SendId(address, bytes,id)) {
		return bytes;
	} else {
		return -1;
	}
}

int NETWORK_Recv(unsigned char *address, unsigned int maxbytes, int id) {
	int ret = ESP01_RecvActiveId(address, maxbytes, id);

	return ret ? ret : -1;
}

void vConnection() {
	xQueueSend = xQueueCreate(5,sizeof(ESP_Message));
	xQueueRecv = xQueueCreate(5,sizeof(ESP_Message));

	ESP01_Init();
	conStatus = STARTING;
	ESP01_Echo(0);
	while(!ESP01_Test());

	while(!NETWORK_Init());
	conStatus = CON;

	ESP_Message message;

	while(1) {
		if(xQueueReceive(xQueueSend, &message, 200)) {
			ESP01_SendId(message.message,message.len,message.id);
		}
	}
}
