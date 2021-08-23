#include "libmemory.h"

struct arg_data {
char loop;
char backdoor;

enum mode_t
{
server, client, sandbox
}mode;

char host [25];
int port;
char path [string_sz];

};


