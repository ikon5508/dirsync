#include <stdio.h>

#include "libmemory.h"

#include "logging.h"


int main (void)
{
init_log ("./tlog.txt");

loggingf (" %d-this is cool-%c, and this too-%s, and this too-\nXXX%d", 50, '$', "wow2", 500);

//loggingf ("\nthis is cool %d, and this too %s, and this too %d\nXXX", 500, "wow!", 50);

  return 0;
}
