#ifndef _IO_H
#define _IO_H

#include <windows.h>
#include <pthread.h>

static SOCKET sock = NULL;
static struct sockaddr_in server_addr = {0};
static int addr_len = sizeof(server_addr);
static pthread_t receive_thread = 0;
static pthread_t send_thread = 0;

void *ReceiveT(void *ParamsT);
void *SendT(void *ParamsT);

static struct _ParamsT {
    int sock;
	char enkey[256];
	int enkey_len;
	pthread_t receive_thread;
	pthread_t send_thread;
} ParamsT;

#endif