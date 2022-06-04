#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Include/server.h"
#include "Include/client.h"

static int mode = -1; // 0 - client, 1 - server, -1 - not specified
static char ip_addr[16] = {0}; // ip-address of host to connection (16 is a max string-length of IPv4 address)
static short int port = 0;

int main(const int argc, const char **argv)
{
	printf("Running Tellus (ZARYA-software)\n\n");

	// Checking number of arguments
	if (argc == 1) {
		printf("help info...\n");
		return 0;
	}

	// Handling arguments
	for (int i = 1; i < argc;) {
		if (!strcmp("-c", argv[i])) {
			mode = 0;

			i++;
			continue;
		}
		
		if (!strcmp("-s", argv[i])) {
			mode = 1;

			i++;
			continue;
		}

		if (!strcmp("-h", argv[i])) {
			if (i + 1 < argc)
				strncpy(ip_addr, argv[i + 1], 16);

			i += 2;
			continue;
		}

		if (!strcmp("-p", argv[i])) {
			if (i + 1 < argc)
				port = (short int) atoi(argv[i + 1]);

			i += 2;
			continue;
		}

		else {
			printf("Unknown key: %s\n", argv[i]);
			i++;
		}
	}

	// Checking mode variable
	if (mode == -1) {
		printf("[-] Mode not specified!\nShutdown...\n");
		return 0;
	}

	// Checking host variable
	if (mode == 0 && ip_addr[6] == '\0') {
		printf("[-] Host address not specified!\nShutdown...\n");
		return 0;
	}

	// Checking port variable
	if (port == 0) {
		printf("[-] Port not specified!\nShutdown...\n");
		return 0;
	}

	// Starting
	printf("[+] Initaliazing successfully!\n");

	switch (mode) {
		case 0:	client(ip_addr, port); break;
		case 1: server(port); break;
	}

	return 0;
}
