/*
 * network.h
 *
 *  Created on: 25/06/2021
 *      Author: josee
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <stdbool.h>


typedef struct esp_message{
	char * message;
	int len;
	int id;
} ESP_Message;


bool NETWORK_Init(void);

bool NETWORK_Connect(const char * type, const char *host, const unsigned short int port, const unsigned short int keepalive, int id);

int NETWORK_Send(unsigned char *address, unsigned int bytes, int id);

int NETWORK_Recv(unsigned char *address, unsigned int maxbytes, int id);

void vConnection();

#endif /* NETWORK_H_ */
