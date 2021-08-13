#include "server.h"
//#include "libmemory.h"
#include "socket.h"

#include "logging.h"

#include "shared.h"


int main (int argc, char **argv)
{
struct sockaddr_in address;
socklen_t addrlen = sizeof(address);


if (debug)
{
char outstr[200];

time_t t;
struct tm *tmp;
t = time(NULL);
tmp = localtime(&t);

//strftime(outstr, 200, "./log/server-%c.txt", tmp);


system ("cd log; rm serv*");

strcpy (outstr, "./log/server-log.txt");
init_log (outstr);

strcpy (outstr, "./log/server-backdoor.txt");

init_sockbackdoor (outstr);

}// if debug

int portno = 9999;
char basepath[200] = ".";
int n;


int outerloop = 0;

int connfd;

// process command line args
for (int i = 1; i < argc; ++i)
{
int argvlen = strlen(argv[i]);

if (!strcmp (argv[i], "-loop"))
{ loggingf (1, "outer loop set\n"); outerloop = 1; continue; }

    int d1, d2;
    // set port# if given
    d1 = atoi (argv[i]);
    d2 = getnext (argv[i], '.', 1, argvlen);
    if (d1 > 0 && d2 == -1)
    portno = d1;
    

        
d1 = getnext (argv[i], '/', 0, argvlen);
        if (d1 > -1)
        {
        if (argv[i][argvlen - 1] == '/')
        {
            midstr (argv[i], basepath, 0, argvlen - 1);
            
        }else{
        
        strcpy (basepath, argv[i]);
         
        } // if / end
        
        } // if / chech
            

        
} // for

loggingf (1, "base path set: %s\n", basepath);   
loggingf (1, "portno: %d\n", portno);
int sv_fd = prepsocket(portno);

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

make_listfiles (&local_list, basepath);

// 1 print local list to log if level at 100
loggingf (100, "<local list=%d>\n%s\n", local_list.procint, local_list.p);

goto skip;

while (outerloop)
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
if (deadtime  >= basetime + 6)
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

// 2 sent local list to remote host
//loggingf (100, "local file list sent to remote host\n%s\n", local_list.p);

} // if <getlist>

if (search (inbuff.p, "<FIN>", 0, inbuff.len)  > -1)
	{loggingf (1, "FIN RECIEVED\n"); break;}

if (search (inbuff.p, "<sendfile=", 0, inbuff.len)  > -1)
	filereciever (&inbuff, basepath, connfd);



} // while session

loggingf (1, "end processes\n");
close (connfd);

	


} // outerloop
if (debug) close_log ();
} // end main
