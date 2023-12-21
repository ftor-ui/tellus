#ifndef _TELLUS_H
#define _TELLUS_H

#include <windows.h>
#include <assert.h>

void handlingArguments(const int argc, const char **argv);
void checkKeysInit(const int argc, const char **argv);

// Temporaly there ...
LPSTR* CommandLineToArgvA(LPSTR lpCmdLine, INT *pNumArgs);

#endif