#include "client.h"
#include "libmemory.h"
#include "socket.h"

#include "shared.h"


//int init_buffer (struct buffer_data *buffer, const int size);


int main (int argc, char **argv)
{

struct sockaddr_in serv_addr;
struct hostent *server = NULL;
int hostset = 0;
int portno = 9999;
char basepath[200] = ".";
int n;

for (int i = 1; i < argc; ++i)
{
int argvlen = strlen(argv[i]);

    //printf ("#%d: %s\n", i, argv[i]);   
    int d1, d2;
    // set port# if given
    d1 = atoi (argv[i]);
    d2 = getnext (argv[i], '.', 1, argvlen);
    if (d1 > 0 && d2 == -1)
    {portno = d1; printf ("portno: %d\n", portno);}
    
//set IP if given
    d1 = getnext (argv[i], '.', 1, argvlen);
    if (d1 > 0)
    {
        d2 = getnext (argv[i], '.', (d1 + 1), argvlen);
            if (d2 > 0)
            {
              server = gethostbyname(argv[i]);
              printf ("host set: %s\n", argv[i]);
            hostset = 1;
                
            }
    }
        
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
    
// generate local file list
    

//printf ("%s:%d\n", local_list.p, local_list.len);


if (!hostset)
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
    
 serv_addr.sin_port = htons(portno);


if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    {printf ("ERROR connecting\n"); exit (0);}


sock_setnonblock (sockfd);

sock_write (sockfd, "<getlist>", 0);

//printf ("%s\n", inbuff.p);

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

make_listfiles (&local_list, basepath);




const int debug = 1;
int tlen = 0;

inbuff.len = sock_read (sockfd, inbuff.p, inbuff.max);
tlen = inbuff.len;

if (debug)
    printf ("len: %d\n", inbuff.len);

//printf ("recieving remote list\n len: %d \n %s\n", inbuff.len, inbuff.p);



int d1, d2;
int session = 0;
char temp[10] = "";

d1 = search (inbuff.p, "<beginlist=", 0, inbuff.len);
d2 = search (inbuff.p, "<endlist>", (d1 + 1), inbuff.len);

//printf ("d1: %d, d2: %d\n", d1, d2);
if (d1 > 0 && d2 > 0)
    session = 1;


if (!session)
//{printf ("remote list incomplete, additional logic required\n%s\n", inbuff.p); exit (0);}
{
strcpy (remote_list.p, inbuff.p);

int loopcount = 0;

while (d2 == -1)
{
inbuff.len = sock_read (sockfd, inbuff.p, inbuff.max);
tlen += inbuff.len;
++loopcount;

if (debug)
    printf ("loop: %d, len: %d\n", loopcount, tlen);

strcat (remote_list.p, inbuff.p);

d2 = search (inbuff.p, "<endlist>", 0, inbuff.len);
} // while   
    
} // if ! listcomplete

remote_list.len = tlen;



if (session)
    strcpy (remote_list.p, inbuff.p);

//if (listcomplete)
//    printf ("list completed!!!\n");

if (debug > 99)
    printf ("%s\n", remote_list.p);

//d1 = '='

//get itemcount and set it for next process
d2 = getnext (remote_list.p, '>', d1, remote_list.len);
midstr (remote_list.p, temp, (d1 + 1), d2);
remote_list.procint = atoi (temp);



make_comparison (&comp_list, remote_list, local_list);

printf ("%s\n", comp_list.p);


//process_complist (comp_list , basepath, sockfd);

//void process_complist (const struct buffer_data comp,const char *basepath, const int fd);

sock_write (sockfd, "<FIN>", 0);

close (sockfd);

}
