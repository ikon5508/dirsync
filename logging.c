#include "logging.h"
#include "libmemory.h"


const int debug = 100;


int fd = 0;

int loggingf (const int level, const char *format, ...)
{
	
if (!level)
	return (0);

va_list ap;

va_start (ap, format);



int formatlen = strlen (format);
char type;
int specify_len = 0;
char entry [maxbuffer] = "";
//char part [maxbuffer] = "";

int len = 0;



int d;
char c;
char *s;


for (int fplace = 0; fplace < formatlen; ++fplace)
{
if (format [fplace] == '%')
{
type = format [fplace + 1];

/*
if (type == '.')
{
	int specify_len = 1;
	
	
} // if type == .
*/

if (type == 's')
{
s = va_arg(ap, char *);

//int slen = strlen (s);

len += sprintf (entry + len, "%s", s);
++fplace;


} // if s
    
if (type == 'd')
{
d = va_arg(ap, int);
len += sprintf (entry + len, "%d", d);
++fplace;
} // if d
    

if (type == 'c')
{
c = (char) va_arg(ap, int);
entry [len] = c;
++len;
entry [len] = 0;
++fplace;    
} // if c
    
    
}else{ // else if    
entry [len] = format [fplace];
++len;

} // if    
} // for

//printf ("%s\n", entry);

va_end(ap);

if (level == 1)
	printf ("%s", entry);
//int idebug = debug;

if (level <= 2 || level == debug)
{

if (fd == 0)
		return 0;

write (fd, entry, len);

} // if

} // loggingf


void close_log ()
{
 close (fd);
 
}

int init_log (const char *path)
{

fd = open (path, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

//printf ("%d:%d\n", fd, errno);

return (fd);

}
