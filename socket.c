#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

//#ifdef __linux__
//#include <linux/sockios.h>
//#endif

#include <fcntl.h>
//#include <signal.h>
//#include <sys/epoll.h>
//#include <sys/ioctl.h>
#include <unistd.h>

//#define _XOPEN_SOURCE 700
//#include <sys/types.h>
//#include <dirent.h>
//#include <sys/sendfile.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <time.h>

const int timeout = 3;



int sock_setnonblock (const int fd)
{
if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) == -1)
{
    perror("calling fcntl");

    return 0;
} // if
    return 1;
} // sock_setnonblock

int prepsocket (const int PORT)
{
int result = 0;
int optval = 1;
struct linger ling;
ling.l_onoff=1;
ling.l_linger=4;

int server_fd = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in address;
int addrlen = sizeof(address);

memset(&address, 0, sizeof (address));

address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons( PORT );
//memset(address.sin_zero, 0, sizeof address.sin_zero);

result = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(int));
if (result == -1)
	perror("error, reuse addr");

//result = setsockopt(server_fd, SOL_SOCKET, TCP_CORK,&optval , sizeof(int));
//if (result == -1)
//	logging("error, TCP-CORK", 100, 0);


//result = setsockopt(server_fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
//if (result == -1)
//	logging("error, linger", 100, 0);

result = bind(server_fd, (struct sockaddr *)&address,(socklen_t) sizeof(address));
if (result == -1)
	perror("error, bind");

result = listen(server_fd, 10);
if (result == -1)
	perror("error, reuse listen");

return (server_fd);
}// end prep socket


int sock_write (const int connfd, const char *buffer, int size)
{
time_t basetime;
time (&basetime);


if (size == 0)
    size = strlen (buffer);

int len = -1;
//nlogging ("bytes queued: ", out.len);

while (len < 0)
{
len = write (connfd, buffer, size);

if (len == -1)
{
usleep (1000);
time_t deadtime;
time (&deadtime);
deadtime -= basetime;

if (deadtime >= timeout)
	{return -1;}

} // if -1

//if (len < out.len

} // while
//nlogging ("bytes written: ", len);
//if (len == out.len)
//{
//if (len < out_max)
//	nlogging ("bytes written: ", len);
//}else{
//nlogging ("bytes queued: ", out.len);
//nlogging ("bytes written: ", len);

//return 0;
//} // logging if
return len;
} // sock_write


int sock_read (const int connfd, char *buffer, const int size)
{
time_t basetime;
time (&basetime);

int len = -1;
//read (connfd, inb->data, in_size);
while (len < 0)
{
len = read (connfd, buffer, size);

if (len == -1)
{
usleep (1000);
time_t deadtime;
time (&deadtime);

deadtime -= basetime;
if (deadtime >= timeout)
	{return -1;}

} // if -1
} // while


return len;
} // sock_read

