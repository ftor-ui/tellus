#include "Include/client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <pthread.h>

static int sock = -1;
static struct sockaddr_in server_addr = {0};
static socklen_t addr_len = sizeof(server_addr);
//static int status = 1;
static pthread_t receive_thread = 0;
static pthread_t send_thread = 0;

static void client_init(char *ip_addr, short int port);
static void client_connect();
static void start_io();

// Prefix 'T' mean that this function will be used of new thread
static void *ReceiveT();
static void *SendT();

void client(char *ip_addr, short int port)
{
	client_init(ip_addr, port);

	client_connect();	
	
	start_io();	
}

static void client_init(char *ip_addr, short int port)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("[-] Socket error!\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip_addr, (void *) &server_addr.sin_addr);
	server_addr.sin_port = htons(port);
}

static void client_connect()
{
	if (connect(sock, (const struct sockaddr *) &server_addr, addr_len) == -1) {
		printf("[-] Connection failure!\n");
		exit(EXIT_FAILURE);
	}

	printf("[+] You connected to server\n\n");
}

static void start_io()
{
	pthread_create(&receive_thread, NULL, &ReceiveT, NULL);
	pthread_create(&send_thread, NULL, &SendT, NULL);

	pthread_join(receive_thread, NULL);
	pthread_join(send_thread, NULL);

	close(sock);
}

static void *ReceiveT()
{
	char message[1024] = {0};
	size_t message_len = 0;

	while (strcmp("/exit", message)) {
		message_len = recv(sock, message, 1023, 0);
		message[message_len] = 0;
	
		if (message_len <= 0)
			break;

		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, message, message_len);
		write(STDOUT_FILENO, "\n> ", 3);
	}

	write(STDOUT_FILENO, "\n[-] Server disconnected\n", 25);

	pthread_cancel(send_thread);
}

static void *SendT()
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

		send(sock, message, message_len, 0);
	}

	pthread_cancel(receive_thread);
}
