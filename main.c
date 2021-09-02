
#include <string.h>
#include "libmemory.h"
#include "main.h"
#include "logging.h"
#include "server.h"
#include "client.h"

int main (int argc, char **argv)
{
struct arg_data args;
memset (&args, 0, sizeof (args));

//set default port and directory
args.port = 9999;
args.path [0] = '.';
// server mode is default mode
//
/* enable debugging data by default */
args.backdoor = 1;
debug = 200;
// enable debugging data by default

// cycle through command line args
for (int i = 1; i < argc; ++i)
{
int argvlen = strlen(argv[i]);

if (!strcmp (argv[i], "-loop"))
{ args.loop = 1; continue; }

if (!strcmp (argv[i], "-backdoor"))
{ args.backdoor = 1; continue; }

if (!strcmp (argv[i], "-sandbox"))
{ args.mode = sandbox; continue; }

if (!strcmp (argv[i], "-client"))
{ args.mode = client; continue; }

if (!strcmp (argv[i], "-debug"))
{ debug = atoi (argv [i + 1]); ++i; continue; }


int d1, d2;
// set port# if given
d1 = atoi (argv[i]);
d2 = getnext (argv[i], '.', 1, argvlen);
if (d1 > 0 && d2 == -1)
{ args.port = d1; continue; }
    
//set IP (host) if given
d1 = getnext (argv[i], '.', 1, argvlen);
if (d1 > 0)
{
d2 = getnext (argv[i], '.', (d1 + 1), argvlen);
if (d2 > 0)
{ strcpy (args.host, argv[i]); continue; }                
} // if d1

d1 = getnext (argv[i], '/', 0, argvlen);
if (d1 > -1)
{
if (argv[i][argvlen - 1] == '/')
{
midstr (argv[i], args.path, 0, argvlen - 1);

}else{

strcpy (args.path, argv[i]);
         
} // if / end
} // if / chech
            
} // for


if (args.mode == server)
	server_mode (args);

if (args.mode == client)
	client_mode (args);

if (args.mode == sandbox)
{
printf ("sandbox_mode\n");

struct arg_data serv_args;
memset (&serv_args, 0, sizeof (serv_args));
serv_args.port = 9999;
strcpy (serv_args.path, "./dir2");
serv_args.backdoor = 1;

struct arg_data client_args;
memset (&client_args, 0, sizeof (client_args));
client_args.mode = client;
client_args.port = 9999;
strcpy (client_args.path, "./dir1/pic.jpg");
client_args.backdoor = 1;
    
int pid = fork ();

//loggingf (1, "hello, PID: %d\n", pid);
//} // if sandbox_mode

if (pid == 0)
{
//loggingf (2, "hello, i child %d\n", pid);
//sleep (1);
client_mode (client_args);
} // if child

if (pid > 0)
{
//loggingf (2, "hello, i parent\n, pid");
server_mode (serv_args);
} // if parent

} // if sandbox
	

} // main
