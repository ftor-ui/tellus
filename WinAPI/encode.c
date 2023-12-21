#include "Include/client.h"

void encode_by_key(char *str, int len, char *enkey, int enkey_len)
{
    if (enkey_len == 0)
        return;
    for (int i = 0; i < len-1; i++) {
        str[i] = str[i] ^ enkey[i % enkey_len];
    }
}