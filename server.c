#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libmemory.h"

#include "socket.h"

#include "logging.h"

#include "shared.h"

#include "main.h"

int server_mode (const struct arg_data args)
{

loggingf (100, "server init\n");

struct sockaddr_in address;
socklen_t addrlen = sizeof(address);

/*
char outstr[200];
time_t t;
struct tm *tmp;
t = time(NULL);
tmp = localtime(&t);

//strftime(outstr, 200, "./log/server-%c.txt", tmp);
*/


if (debug)
{
system ("cd log; rm serv*");
init_log ("./log/server-log.txt");

if (args.backdoor)
	init_sockbackdoor ("./log/server-backdoor.txt");

}// if debug


int connfd;

loggingf (1, "base path set: %s\n", args.path);   
loggingf (1, "portno: %d\n", args.port);
int sv_fd = prepsocket(args.port);

char inb[maxbuffer];
struct buffer_data inbuff;
inbuff.p = inb;
inbuff.max = maxbuffer;

char rmlist[maxbuffer];
struct buffer_data remote_list;
remote_list.p = rmlist;
remote_list.max = maxbuffer;

char locallist[maxbuffer];
struct buffer_data local_list;
local_list.p = locallist;
local_list.max = maxbuffer;

make_listfiles (&local_list, args.path);

// 1 print local list to log if level at 100
loggingf (100, "<local list=%d>\n%s\n", local_list.procint, local_list.p);

goto skip;

while (args.loop)
{
skip:

connfd = accept(sv_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

sock_setnonblock (connfd);
int session = 1;

time_t basetime;
time (&basetime);

while (session)
{
inbuff.len = sock_read (connfd, inbuff.p, inbuff.max);

if (inbuff.len > 0)
	time (&basetime);

if (inbuff.len == -1)
{loggingf (1, "server timeout = -1\n"); break;}

if (inbuff.len == 0)
{
time_t deadtime;
time (&deadtime);

// if client stalls boot connection
if (deadtime  >= basetime + 2)
{loggingf (1, "client stalled\n"); break;}
}


if (!strcmp (inbuff.p, "<getlist>"))
{
session = 2;
char header [100];
sprintf (header, "<beginlist=%d>\n", local_list.procint);

sock_write (connfd, header, 0);
sock_write (connfd, local_list.p , local_list.len);
sock_write (connfd, "<endlist>" , 0);

} // if <getlist>

if (search (inbuff.p, "<FIN>", 0, inbuff.len)  > -1)
	{loggingf (1, "FIN RECIEVED\n"); break;}

if (search (inbuff.p, "<sendfile=", 0, inbuff.len)  > -1)
	filereciever (&inbuff, args.path, connfd);



} // while session

loggingf (1, "end processes\n");
close (connfd);

	


} // outerloop
if (debug) close_log ();
} // end main
