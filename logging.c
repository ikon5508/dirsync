#include "logging.h"
#include "libmemory.h"


int fd;

int loggingf (const char *format, ...)
{
// does not accept modulator as first charactor
va_list ap;

va_start (ap, format);



int formatlen = strlen (format);

int count = 0;
int modulo = 0;
char type;
char entry [maxbuffer] = "";
char part [maxbuffer];
int oldmodulo = 0;


int len;


int d;
char c;
char *s;

do {
oldmodulo = modulo;

if (modulo > 0)
	modulo = getnext (format, '%', modulo + 1, formatlen);

if (modulo == 0)
	{modulo = getnext (format, '%', 0, formatlen);
	/*printf ("first-%d\n", modulo);*/}

if (modulo > -1)
{
++count;
type = format [modulo + 1];

	if(oldmodulo > 0)
	{
	midstr (format, part, oldmodulo + 2, modulo);
	strcat (entry, part);
	
	} // if oldmodulo > 0

	if(oldmodulo == 0)
	{
	midstr (format, part, 0, modulo);
	strcat (entry, part);
	printf ("first-%d\n", modulo);
	} // if oldmodulo > 0



if (type == 's')
{
s = va_arg(ap, char *);
strcat (entry, s);

} // if s

if (type == 'd')
{
d = va_arg(ap, int);
sprintf (part, "%d", d);
strcat (entry, part);

} // if d


if (type == 'c')
{
c = (char) va_arg(ap, int);
//strcat (entry, c);
len = strlen (entry);
entry [len] = c;
} // if c


//printf ("%d: %s\n", count, part);


} // if
} while (modulo > 0);

//printf ("%s\n", entry);

va_end(ap);

len = strlen (format);

midstr (format, part, oldmodulo + 2, len);
strcat (entry, part);
len = strlen (entry);

write (fd, entry, len);


} // loggingf



int close_log ()
{
close (fd);

}



int init_log (const char *path)
{
fd = open (path, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

//printf ("%d:%d\n", fd, errno);

return (fd);

}
