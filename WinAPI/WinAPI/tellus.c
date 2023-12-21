#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Include/tellus.h"
#include "Include/server.h"
#include "Include/client.h"

#define _MODE_CLIENT 0
#define _MODE_SERVER 1
#define _MODE_NS -1
#define _PORT_NS 0

static int mode = _MODE_NS; // 0 - client, 1 - server, -1 - not specified
static char ip_addr[16] = {0}; // ip-address of host to connection (16 is a max string-length of IPv4 address)
static short int port = _PORT_NS;

void printHelpInfo()
{
	printf("Using: tellus [-key] [info/opt] ...\n\n\
       Keys:      Info for key:            Description:\n\n\
         -c                                Choose client mode. Client mode meaning connecting to the server.\n\
	 				   Required keys: '-h' - host, '-p' - port.\n\n\
         -s                                Choose server mode. Server mode meaning that you would be server host.\n\
	                                   Required keys: '-p' - port.\n\n\
	 -h       IPv4-address             IP-address of host that you would be connected.\n\n\
	 -p       Port-address             Port address should be specified that you could be connected to server host or\n\
	                                   created server host.\n\n");
	exit(EXIT_SUCCESS);
}

int main(const int argc, const char **argv)
{
	printf("Running Tellus (ZARYA-software)\n\n");

	// Checking number of arguments
	if (argc == 1)
		printHelpInfo();
	

	// Handling arguments
	handlingArguments(argc, argv);

	// Check keys initialization
	checkKeysInit(argc, argv);

	
	switch (mode) {
		case _MODE_CLIENT: client(ip_addr, port); break;
		case _MODE_SERVER: server(port); break;
	}

	return 0;
}

void handlingArguments(const int argc, const char **argv)
{
	for (int i = 1; i < argc;) {
		if (!strcmp("-c", argv[i])) {
			mode = _MODE_CLIENT;

			i++;
			continue;
		}
		
		if (!strcmp("-s", argv[i])) {
			mode = _MODE_SERVER;

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
}

void checkKeysInit(const int argc, const char **argv)
{
	// Checking mode variable
	if (mode == _MODE_NS) {
		printf("[-] Mode not specified!\nShutdown...\n");
		exit(EXIT_FAILURE);
	}

	// Checking host variable
	if (mode == _MODE_CLIENT && ip_addr[6] == '\0') {
		printf("[-] Host address not specified!\nShutdown...\n");
		exit(EXIT_FAILURE);
	}

	// Checking port variable
	if (port == _PORT_NS) {
		printf("[-] Port not specified!\nShutdown...\n");
		exit(EXIT_FAILURE);
	}

	// Starting
	printf("[+] Initaliazing successfully!\n");
}
