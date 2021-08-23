
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>


#include "libmemory.h"
#include "socket.h"
#include "logging.h"
#include "shared.h"

#include "main.h"

int client_mode (const struct arg_data args)
{

struct sockaddr_in serv_addr;
struct hostent *server = NULL;

/*
char outstr[200];
time_t t;
struct tm *tmp;
t = time(NULL);
tmp = localtime(&t);
//strftime(outstr, 200, "./log/client-%c.txt", tmp);
*/

if (debug)
{
system ("cd log; rm cli*");
init_log ("./log/client-log.txt");

if (args.backdoor)
	init_sockbackdoor ("./log/client-backdoor.txt");


}// if debug


if (args.host [0] == 0)
{
server = gethostbyname("localhost"); // get hostbyname
//printf ("host set to localhost\n");
    
if (server == NULL) {
    
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
    }
} // if hostset


int sockfd;

sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        perror("ERROR opening socket");


bzero((char *) &serv_addr, sizeof(serv_addr));
 serv_addr.sin_family = AF_INET;

 bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    
 serv_addr.sin_port = htons(args.port);


if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    {printf ("ERROR connecting\n"); exit (0);}


sock_setnonblock (sockfd);

sock_write (sockfd, "<getlist>", 0);

// 1 getlist sent
loggingf (100, "sent: <getlist>\n");

char inb[maxbuffer];
struct buffer_data inbuff;
inbuff.p = inb;
inbuff.max = maxbuffer;

char rmlist[maxbuffer] = "";
struct buffer_data remote_list;
remote_list.p = rmlist;
remote_list.max = maxbuffer;

char complist[maxbuffer] = "";
struct buffer_data comp_list;
comp_list.p = complist;
comp_list.max = maxbuffer;

char locallist[maxbuffer];
struct buffer_data local_list;
local_list.p = locallist;
local_list.max = maxbuffer;

make_listfiles (&local_list, args.path);

// print local list
loggingf (100, "<local list=%d>\n%s\n", local_list.procint, local_list.p);

inbuff.len = sock_read (sockfd, inbuff.p, inbuff.max);

int d1, d2;
int session = 0;
char temp[100] = "";

d1 = search (inbuff.p, "<beginlist=", 0, inbuff.len);
d2 = search (inbuff.p, "<endlist>", (d1 + 1), inbuff.len);

//printf ("d1: %d, d2: %d\n", d1, d2);
if (d1 > 0 && d2 > 0)
    session = 1;


if (!session)
{
remote_list.len = midstr (inbuff.p, remote_list.p, d1 + 4, inbuff.len);

// search for > so we can pick out the remote itemcount
d2 = getnext (inbuff.p, '>', d1, inbuff.len);
midstr (inbuff.p, temp, d1 + 1, d2);
remote_list.procint = atoi (temp);

d2 = -1;
while (d2 == -1)
{
inbuff.len = sock_read (sockfd, inbuff.p, inbuff.max);

strcat (remote_list.p, inbuff.p);
remote_list.len += inbuff.len;

//loggingf (100, "added: %s\n", inbuff.p);

d2 = search (inbuff.p, "<endlist>", 0, inbuff.len);
} // while   

} // if ! session listcomplete

if (session) 
{
//loggingf (100, "reciever complete <single read>\n");
remote_list.len = midstr (inbuff.p, remote_list.p, d1 + 4, inbuff.len - 10);

//search for next > so we can pickout the itemcount
d2 = getnext (inbuff.p, '>', d1, inbuff.len);
midstr (inbuff.p, temp, d1 + 1, d2);
remote_list.procint = atoi (temp);
} // if session

// multipart reciever will still include <endlist>
loggingf (100, "\n...\n1=1 read, 0=~ multipart read: %d\n%.*s\n...\n", session, remote_list.len, remote_list.p);
// multipart reciever will still include <endlist>


struct comp_data compdata = make_comparison (&comp_list, remote_list, local_list);
loggingf (100, "\n...\ncomp list\n%s\n...\n", comp_list.p);

loggingf (1, "need to get: %d files : %d bytes\nneed to send: %d files: %d bytes\n", compdata.filesin, compdata.bytesin, compdata.filesout, compdata.bytesout);

//process_complist (comp_list, basepath, sockfd);


//exit (0);
sock_write (sockfd, "<FIN>", 0);

close (sockfd);
close_log ();

}
