
/*
 -loop
 IP
 Port
 Path
 
 
 */
#include <string.h>
#include "libmemory.h"
#include "main.h"
#include "logging.h"

int main (int argc, char **argv)
{

struct arg_data args;
memset (&args, 0, sizeof (args));

// cycle through command line args
for (int i = 1; i < argc; ++i)
{
int argvlen = strlen(argv[i]);

if (!strcmp (argv[i], "-loop"))
{ args.loop = 1; continue; }

if (!strcmp (argv[i], "-sandbox"))
{ args.mode = sandbox; continue; }

if (!strcmp (argv[i], "-client"))
{ args.mode = sandbox; continue; }

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

printf ("debug: %d\n", debug);
printf ("port: %d\n", args.port);
printf ("path: %s\n", args.path);
printf ("host: %s\n", args.host);

printf ("mode: %d\n", args.mode);

} // main
