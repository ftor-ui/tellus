#include "Include/client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void client(char *ip_addr, short int port)
{
	int sock = -1;
	struct sockaddr_in server_addr = {0};
	socklen_t addr_len = sizeof(server_addr);

        sock = socket(AF_INET, SOCK_STREAM, 0);

	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip_addr, (void *) &server_addr.sin_addr);
	server_addr.sin_port = htons(port);
	
	if (connect(sock, (const struct sockaddr *) &server_addr, addr_len) == -1) {
		printf("[-] Connection failure!\n");
		exit(EXIT_FAILURE);
	}

	printf("[+] You connected to server\n\n");

	char message[1024] = {0};
	size_t message_len = 0;

	while (strcmp("/exit", message)) {
		message_len = recv(sock, message, 1024, 0);

		if (message_len <= 0) {
			printf("\n[-] Server disconnected\n");
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

		send(sock, message, message_len, 0);
	}

	close(sock);
}
