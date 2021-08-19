#include <stdio.h>

#include "libmemory.h"

#include "logging.h"

#include "shared.h"


int main (void)
{
char *s = "this is a cool string";

char new [100];
memmove (new, s, 5);

printf ("%s\n", s);
	
	
	
	
	exit (0);
init_log ("./log/tlog.txt");

char rmlist[maxbuffer];
struct buffer_data remote_list;
remote_list.p = rmlist;
remote_list.max = maxbuffer;

char rmlist1[maxbuffer];
struct buffer_data comp_list;
comp_list.p = rmlist1;
comp_list.max = maxbuffer;

char rmlist2[maxbuffer];
struct buffer_data local_list;
local_list.p = rmlist2;
local_list.max = maxbuffer;






make_listfiles (&local_list, "./dir1");
make_listfiles (&remote_list, "./dir2");



//remote_list.procint += 5;
//local_list.procint += 5;


make_comparison (&comp_list, remote_list, local_list);


printf ("lpcal: %s\n", local_list.p);
printf ("remote %s\n", remote_list.p);


printf ("comp %s\n", comp_list.p);






close_log ();

  return 0;
}
