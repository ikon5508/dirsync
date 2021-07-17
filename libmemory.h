
#include <stdio.h>

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



//#include <stdio.h>
//#include <sys/socket.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <netinet/in.h>
//#include <string.h>
//#include <sys/ioctl.h>
//#define PORT 9999;

//#define _XOPEN_SOURCE 700
//#include <sys/types.h>
//#include <dirent.h>
//#include <sys/sendfile.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <arpa/inet.h>
//#include <netinet/tcp.h>

//#ifdef __linux__
//#include <linux/sockios.h>
//#endif
//#include <fcntl.h>
//#include <signal.h>



#ifndef libfunc
#define libfunc 2

struct buffer_data
{
char *p;
int procint;
int len;
int max;
};



int init_buffer (struct buffer_data *buffer, const int size);
int getnext (const char *str, const char next, const int start, const int end);
int midstr(const char *major, char *minor, int start, int end);
int getlast (const char *str, const char next, const int end);


//const int maxbuffer = 100000;
int search (const char *main, const char *minor, int start, int end);


#endif

#ifndef buffsz
#define buffsz
#define maxbuffer 100000

#define string_sz 1000

#endif

#ifndef delim_char
#define delim_char
#define delimchar 21
#endif



