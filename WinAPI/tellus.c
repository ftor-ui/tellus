#include <windows.h>

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
static char enkey[256] =  {NULL}; // Encode-Key
static int enkey_len = 0;

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
	                                   created server host.\n\n\
	 -k 	  Encode-Key   	           Key that will be encode message packets\n\n");
	exit(EXIT_SUCCESS);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR strCmdLine, int nCmdShow)
{
	int w_argc = 0;
	LPWSTR *w_argv = CommandLineToArgvA(GetCommandLineA(), &w_argc);
	printf("Running Tellus (ZARYA-software)\n\n");

	// Checking number of arguments
	if (w_argc == 1)
		printHelpInfo();
	

	// Handling arguments
	handlingArguments(w_argc, w_argv);

	// Check keys initialization
	checkKeysInit(w_argc, w_argv);

	
	switch (mode) {
		case _MODE_CLIENT: client(ip_addr, port, enkey, enkey_len); break;
		case _MODE_SERVER: server(port, enkey, enkey_len); break;
	}

	return 0;
}

void handlingArguments(const int w_argc, const char** w_argv)
{
	for (int i = 1; i < w_argc;) {
		if (!strcmp("-c", w_argv[i])) {
			mode = _MODE_CLIENT;

			i++; 
			continue;
		}
		
		if (!strcmp("-s", w_argv[i])) {
			mode = _MODE_SERVER;

			i++;
			continue;
		}

		if (!strcmp("-h", w_argv[i])) {
			if (i + 1 < w_argc)
				strncpy(ip_addr, w_argv[i + 1], 16);

			i+=2;
			continue;
		}

		if (!strcmp("-p", w_argv[i])) {
			if (i + 1 < w_argc)
				port = (short int) atoi(w_argv[i + 1]);

			i+=2;
			continue;
		}

		if (!strcmp("-k", w_argv[i])) {
			if (i + 1 < w_argc)
				strncpy(enkey, w_argv[i + 1], 256);
			
			enkey_len = strlen(enkey);

			i+=2;
			continue;
		}

		else {
			printf("Unknown key: %s\n", w_argv[i]);
			i++;
		}
	}
}

void checkKeysInit(const int w_argc, const char **w_argv)
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

LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT *pNumArgs)
{
    int retval;
    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, NULL, 0);
    if (!SUCCEEDED(retval))
        return NULL;
    LPWSTR lpWideCharStr = (LPWSTR)malloc(retval * sizeof(WCHAR));
    if (lpWideCharStr == NULL)
        return NULL;
    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, lpCmdLine, -1, lpWideCharStr, retval);
    if (!SUCCEEDED(retval))
    {
        free(lpWideCharStr);
        return NULL;
    }
    int numArgs;
    LPWSTR* args;
    args = CommandLineToArgvW(lpWideCharStr, &numArgs);
    free(lpWideCharStr);
    if (args == NULL)
        return NULL;
    int storage = numArgs * sizeof(LPSTR);
    for (int i = 0; i < numArgs; ++ i)
    {
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, NULL, 0, NULL, &lpUsedDefaultChar);
        if (!SUCCEEDED(retval))
        {
            LocalFree(args);
            return NULL;
        }
        storage += retval;
    }
    LPSTR* result = (LPSTR*)LocalAlloc(LMEM_FIXED, storage);
    if (result == NULL)
    {
        LocalFree(args);
        return NULL;
    }
    int bufLen = storage - numArgs * sizeof(LPSTR);
    LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
    for (int i = 0; i < numArgs; ++ i)
    {
        assert(bufLen > 0);
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[i], -1, buffer, bufLen, NULL, &lpUsedDefaultChar);
        if (!SUCCEEDED(retval))
        {
            LocalFree(result);
            LocalFree(args);
            return NULL;
        }
        result[i] = buffer;
        buffer += retval;
        bufLen -= retval;
    }
    LocalFree(args);
    *pNumArgs = numArgs;
    return result;
}