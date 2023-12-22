#include "Include/server.h"
#include "Include/io.h"
#include "Include/encode.h"

#include <windows.h>
#include <winsock.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

static void server_init(short int port);
static void server_wait();
static void start_io();

void server(short int port, char *enkey, int enkey_len)
{
	strncpy(ParamsT.enkey,enkey,256);
	ParamsT.enkey_len = enkey_len;
	server_init(port);	
	
	server_wait();

	start_io();
}

static void server_init(short int port)
{
	WORD sockVer = MAKEWORD(2,2);
	WSADATA wsaData = {0};
	WSAStartup(sockVer, &wsaData);
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("[-] Socket error!\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (bind(sock, (struct sockaddr*) &server_addr, addr_len) == SOCKET_ERROR) {
		printf("[-] Bind error!\n");
		exit(EXIT_FAILURE);
	}

	if (listen(sock, 1) == SOCKET_ERROR) {
		printf("[-] Listen error!\n");
		exit(EXIT_FAILURE);
	}
}

static void server_wait()
{
	ParamsT.sock = accept(sock, (struct sockaddr*) &server_addr, &addr_len);

	if (ParamsT.sock == -1) {
		printf("[-] Accept error!");
		exit(EXIT_FAILURE);
	}

	printf("[+] Client conected\n\n");
}

static void start_io()
{
	pthread_create(&ParamsT.receive_thread, NULL, &ReceiveT, (void *)&ParamsT);
	pthread_create(&ParamsT.send_thread, NULL, &SendT, (void *)&ParamsT);
	
	pthread_join(ParamsT.receive_thread, NULL);
	pthread_join(ParamsT.send_thread, NULL);

	closesocket(ParamsT.sock);
	closesocket(sock);
	
	WSACleanup();
}