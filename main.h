#include "libmemory.h"

struct arg_data {
int loop;

enum mode_t
{
none, server, client, sandbox
}mode;

char host [25];
int port;
char path [string_sz];

};


