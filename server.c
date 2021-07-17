#include "server.h"
#include "libmemory.h"
#include "socket.h"

#include "shared.h"


int main (int argc, char **argv)
{
struct sockaddr_in address;
socklen_t addrlen = sizeof(address);

int portno = 9999;
char basepath[200] = ".";
int n;

const int debug = 1;
        
int outerloop = 0;

int connfd;

// process command line args
for (int i = 1; i < argc; ++i)
{
int argvlen = strlen(argv[i]);

if (!strcmp (argv[i], "-loop"))
{ printf ("outer loop set\n"); outerloop = 1; continue; }

    int d1, d2;
    // set port# if given
    d1 = atoi (argv[i]);
    d2 = getnext (argv[i], '.', 1, argvlen);
    if (d1 > 0 && d2 == -1)
    {portno = d1; printf ("portno: %d\n", portno);}
    

        
d1 = getnext (argv[i], '/', 0, argvlen);
        if (d1 > -1)
        {
        if (argv[i][argvlen - 1] == '/')
        {
            midstr (argv[i], basepath, 0, argvlen - 1);
            
        }else{
        
        strcpy (basepath, argv[i]);
         
        }
        
         printf ("base path set: %s\n", basepath);   
        }
            

        
} // for


int sv_fd = prepsocket(portno);


goto skip;

while (outerloop)
{
skip:


connfd = accept(sv_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

sock_setnonblock (connfd);
    

//printf ("%s\n", local_list.p);

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


inbuff.len = sock_read (connfd, inbuff.p, inbuff.max);
int session = 0;

if (!strcmp (inbuff.p, "<getlist>"))
    session = 1;

if (debug)
    printf ("session: %d\n", session);

int tlen = 0;
int headlen;

char header [10];

headlen = sprintf (header, "<beginlist=%d>\n", local_list.procint);

tlen = sock_write (connfd, header, headlen);
tlen += sock_write (connfd, local_list.p , local_list.len);
tlen += sock_write (connfd, "<endlist>" , 0);

while (session)
{
inbuff.len = sock_read (connfd, inbuff.p, inbuff.max);
	






	
	int d2 = search (inbuff.p, "<FIN>", 0, inbuff.len);
	if (d2 > -1)
		break;
	
} // session loop

session = 0;

printf ("all processes completed\n");

close (connfd);
} // outerloop

//  sleep (10);
} // end main
