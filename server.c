#include "Include/server.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void server(short int port)
{
	int server_sock = -1;
	struct sockaddr_in server_addr = {0};
	socklen_t addr_len = sizeof(server_addr);
	int client_sock = -1;
	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	bind(server_sock, (struct sockaddr*) &server_addr, addr_len);

	listen(server_sock, 1);

	client_sock = accept(server_sock, (struct sockaddr*) &server_addr, &addr_len);

	printf("[+] Client conected\n\n");

	char message[1024] = {0};
	size_t message_len = 0;

	message_len = send(client_sock, "Welcome from server!\n", 21, 0);

	while (strcmp("/exit", message)) {
		message_len = recv(client_sock, message, 1024, 0);

		if (message_len <= 0) {
			printf("\n[-] Client disconnected\n");
			break;
		}

		write(STDOUT_FILENO, message, message_len);
		
		printf("\n> ");

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
		
	close(client_sock);
	close(server_sock);
}
