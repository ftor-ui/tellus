#include "Include/server.h"

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pthread.h>

static int server_sock = -1;
static struct sockaddr_in server_addr = {0};
static socklen_t addr_len = sizeof(server_addr);
static int client_sock = -1;
//static int status = 1;
static pthread_t receive_thread = 0;
static pthread_t send_thread = 0;

// Prefix 'T' mean that this function will be used of new thread
static void *ReceiveT();
static void *SendT();

void server(short int port)
{
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	bind(server_sock, (struct sockaddr*) &server_addr, addr_len);

	listen(server_sock, 1);

	client_sock = accept(server_sock, (struct sockaddr*) &server_addr, &addr_len);

	printf("[+] Client conected\n\n");

	pthread_create(&receive_thread, NULL, &ReceiveT, NULL);
	pthread_create(&send_thread, NULL, &SendT, NULL);

	pthread_join(receive_thread, NULL);
	pthread_join(send_thread, NULL);
		
	close(client_sock);
	close(server_sock);
}

static void *ReceiveT()
{
	char message[1024] = {0};
	size_t message_len = 0;

	while (strcmp("/exit", message)) {
		message_len = recv(client_sock, message, 1023, 0);
		message[message_len] = 0;
	
		if (message_len <= 0)
			break;

		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, message, message_len);
		write(STDOUT_FILENO, "\n> ", 3);
	}

	write(STDOUT_FILENO, "\n[-] Client disconnected\n", 25);

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

		send(client_sock, message, message_len, 0);
	}

	pthread_cancel(receive_thread);
}
