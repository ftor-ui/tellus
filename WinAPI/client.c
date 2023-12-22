#include "Include/client.h"
#include "Include/io.h"
#include "Include/encode.h"

#include <windows.h>
#include <winsock.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

static void client_init(char *ip_addr, short int port);
static void client_connect();
static void start_io();

void client(char *ip_addr, short int port, char *enkey, int enkey_len)
{
	strncpy(ParamsT.enkey,enkey,256);
	ParamsT.enkey_len = enkey_len;
	client_init(ip_addr, port);

	client_connect();	
	
	start_io();	
}

static void client_init(char *ip_addr, short int port)
{
	WORD sockVer = MAKEWORD(2,2);
	WSADATA wsaData = {0};
	WSAStartup(sockVer, &wsaData);

	ParamsT.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ParamsT.sock == INVALID_SOCKET) {
		printf("[-] Socket error!\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip_addr, (void *) &server_addr.sin_addr);
	server_addr.sin_port = htons(port);
}

static void client_connect()
{
	if (connect(ParamsT.sock, (const struct sockaddr *) &server_addr, addr_len) == SOCKET_ERROR) {
		printf("[-] Connection failure!\n");
		exit(EXIT_FAILURE);
	}

	printf("[+] You connected to server\n\n");
}

static void start_io()
{
	pthread_create(&ParamsT.receive_thread, NULL, &ReceiveT, (void *)&ParamsT);
	pthread_create(&ParamsT.send_thread, NULL, &SendT, (void *)&ParamsT);

	pthread_join(ParamsT.receive_thread, NULL);
	pthread_join(ParamsT.send_thread, NULL);

	closesocket(ParamsT.sock);
	WSACleanup();
}