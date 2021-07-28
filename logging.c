#include "logging.h"
#include "libmemory.h"


int fd;

int loggingf (const char *format, ...)
{
// does not accept modulator as first charactor
va_list ap;

va_start (ap, format);



int formatlen = strlen (format);
char type;
char specify_len = 0;
char entry [string_sz] = "";
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

write (fd, entry, len);

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
