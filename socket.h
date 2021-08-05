#include <time.h>

#include "logging.h"

int prepsocket (const int PORT);
int sock_setnonblock (const int fd);

int sock_write (const int connfd, const char *buffer, const int size);
int sock_read (const int connfd, char *buffer, int size);

int sendfile (const char *path, const int fd);

