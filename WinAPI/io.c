#include "Include/io.h"
#include "Include/encode.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void *ReceiveT(void *ParamsT)
{
	char message[1024] = {0};
	int message_len = 0;

	while (strcmp("/exit", message)) {
        message_len = recv(((struct _ParamsT*)ParamsT)->sock, message, 1023, 0);
		message[message_len] = 0;
		encode_by_key(message,message_len,((struct _ParamsT*)ParamsT)->enkey,((struct _ParamsT*)ParamsT)->enkey_len);
	
		if (message_len <= 0)
			break;
        
		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, message, message_len);
		write(STDOUT_FILENO, "\n> ", 3);
	}

	write(STDOUT_FILENO, "\n[-] Other side disconnected\n", 29);

	pthread_cancel(((struct _ParamsT*)ParamsT)->send_thread);
}

void *SendT(void *ParamsT)
{
	char message[1024] = {0};
	size_t message_len = 0;

	while (strcmp("/exit", message)) {
		write(STDOUT_FILENO, "\n> ", 3);

		message_len = 0;
		for (int i = 0; i < 1023; i++) {
			message[i] = getc(stdin);

			if (message[i] == '\n') {
				message[i] = 0;
				break;
			}

			message_len++;
		}

		encode_by_key(message,message_len,((struct _ParamsT*)ParamsT)->enkey,((struct _ParamsT*)ParamsT)->enkey_len);
		send(((struct _ParamsT*)ParamsT)->sock, message, message_len, 0);
	}

	pthread_cancel(((struct _ParamsT*)ParamsT)->receive_thread);
}
