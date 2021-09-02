
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>



#include "libmemory.h"
#include "socket.h"
#include "shared.h"
#include "logging.h"

//#include "client.h"
//#include "server.h"

/*
1 standard debug file(s)
100 detailed server / client () info
200 make_comparison info
*/

//replaced with args
//const int debug = 200;
//replaced with args

int filereciever (struct buffer_data *inbuff, const char *basepath, const int connfd)
{

char fname [string_sz];
char fullpath [string_sz];
char str_sz [100];

int status = 0;

int d1 = getlast (inbuff->p, ';', inbuff->len);
// <sendfile=
midstr (inbuff->p, fname, 10, d1);

int d2 = getnext (inbuff->p, '>', d1, inbuff->len);
midstr (inbuff->p, str_sz, d1 + 1, d2);

size_t fsize = atoi (str_sz);
size_t progress = 0;

loggingf (200, "file size: %d\n", fsize);


sprintf (fullpath, "%s/%s", basepath, fname);

int localfd = open (fullpath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

loggingf (200, "full path: %s, localfd: %d\n", fullpath, localfd);

if (status == 0)
{
char temp [inbuff->len];
int len = midstr (inbuff->p, temp, d2 + 1, inbuff->len);
write (localfd, temp, len);
progress += len;
status = 1;
loggingf (200, "status 0, len: %d\n", len);

} // if status = 0

while (progress < fsize)
{
inbuff->len = sock_read (connfd, inbuff->p, inbuff->max);
write (localfd, inbuff->p, inbuff->len);
progress += inbuff->len;

loggingf (200, "progress: %d\n", progress);

} // while

//loggingf (1, "[%s], %s--%s\n", inbuff->p, fname, str_sz);
close (localfd);
exit (0);



}
void process_complist (const struct buffer_data comp, const char *basepath, const int fd)
{

int start_line;
int end_line = -1;
	
char entry [string_sz];
char path [string_sz];
char fullpath [string_sz];

char temp1 [string_sz];
char ftype;
char action;

int dftype;
int dsize;

char size [100];

int itemcount = 0;

while (itemcount < comp.procint)
{
//"%s%s;%c;%d\n", action, temp1, local_ftype [loc], local_sz [loc]);
//loggingf (100, "itemcount: %d\n", itemcount);

start_line = end_line + 1;
end_line = getnext (comp.p, (char) 10, start_line, comp.len);

dftype = getnext (comp.p, ';', start_line, end_line);
dsize = getnext (comp.p, ';', (dftype + 1), end_line);

++itemcount;

// set variables
midstr (comp.p, path, start_line + 4, dftype);
midstr (comp.p, size, dsize, end_line);

ftype = comp.p [dftype + 1];
action = comp.p[start_line];

sprintf (fullpath, "%s/%s", basepath, path);


loggingf (100, "%d: path: (%s), action: (%c): size: (%s)\n",itemcount, path, action, size);
/*
if (action == 's')
{
sprintf (entry, "<sendfile=%s;%c;%s>", path, ftype, size);
sock_write (fd, entry, 0);
loggingf (1, "%s\n", entry);

sendfile (fullpath, fd); 
} // if s
*/
// not stragetigized yet
/*
if (action == 'g')
{
sprintf (entry, "<getfile=%s>", path);
sock_write (fd, entry, 0);

// poor design strategy
getfile (fullpath, fd);
// fullpath !!!
} // if g
*/

}	// while
} // process process_complist





struct comp_data make_comparison (struct buffer_data *comp, const struct buffer_data remote, const struct buffer_data local)
{
struct comp_data rtn;
rtn.bytesin = 0;
rtn.bytesout = 0;
rtn.filesin = 0;
rtn.filesout = 0;

int local_start [local.procint];
int remote_start [remote.procint];

char local_stat [local.procint];
char remote_stat [remote.procint];

int local_pathlen [local.procint];
int remote_pathlen [remote.procint];

time_t local_mtime [local.procint];
time_t remote_mtime [remote.procint];

char local_ftype [local.procint];
char remote_ftype [remote.procint];

size_t local_sz [local.procint];
size_t remote_sz [remote.procint];

int end_line = -1;
int start_path;

int dftype, dmtime, dsz, end_path, entrylen;

char temp1 [string_sz];
char temp2 [string_sz];

int itemcount = 0;

// init touch vars
for (int i = 0; i < local.procint; ++i)
    local_stat [i] = 'u';

for (int i = 0; i < remote.procint; ++i)
    remote_stat [i] = 'u';

const char *action;
const char *send = "snd:";
const char *get = "get:";

loggingf (200, "itemcount local: %d, remote: %d\n", local.procint, remote.procint);


// local iteration loop
while (itemcount < local.procint)
{
start_path = end_line +1;
local_start [itemcount] = start_path;

    end_line = getnext (local.p, (char) 10, end_line + 1, local.len);
        if (end_line == -1)
        {break;}
    
    dftype = getnext (local.p, ';', start_path, end_line);
        local_pathlen [itemcount] = dftype - start_path;

    dmtime = getnext (local.p, ';', dftype + 1, end_line);
    
    dsz = getnext (local.p, ';', dmtime + 1, end_line);
    
    // set mtime
    midstr (local.p, temp1, dmtime + 1, dsz);
    //printf ("%d:%d: %s\n", dmtime, dsz, temp);
    local_mtime [itemcount] = atoi (temp1);
    
    // set ftype
    midstr (local.p, temp1, dftype + 1, dftype + 2);
    //printf ("%s\n", temp);
        local_ftype [itemcount] = temp1[0];
    
    // set path
    //midstr (local.p, temp, start_path, dftype);
    //printf ("%s\n", temp);
    
    //set size
    midstr (local.p, temp1, dsz + 1, end_line);
    //printf ("%s\n", temp);
    local_sz [itemcount] = atoi (temp1);
    
++itemcount;
} // local iteration

// remote iteration loop
end_line = -1;
itemcount = 0;
while (itemcount < remote.procint)
{
start_path = end_line +1;
remote_start [itemcount] = start_path;


    end_line = getnext (remote.p, (char) 10, end_line + 1, remote.len);
        if (end_line == -1)
        {break;}
    
    dftype = getnext (remote.p, ';', start_path, end_line);
        remote_pathlen [itemcount] = dftype - start_path;

    dmtime = getnext (remote.p, ';', dftype + 1, end_line);
    
    dsz = getnext (remote.p, ';', dmtime + 1, end_line);
    
    // set mtime
    midstr (remote.p, temp1, dmtime + 1, dsz);
    //printf ("%d:%d: %s\n", dmtime, dsz, temp);
    remote_mtime [itemcount] = atoi (temp1);
    
    // set ftype
    midstr (remote.p, temp1, dftype + 1, dftype + 2);
    //printf ("%s\n", temp);
    remote_ftype [itemcount] = temp1[0];
    
    // set path
    //midstr (remote.p, temp, start_path, dftype);
    //printf ("%s\n", temp);
    
    //set size
    midstr (remote.p, temp1, dsz + 1, end_line);
    //printf ("%s\n", temp);
    remote_sz [itemcount] = atoi (temp1);
    
++itemcount;
} // remote iteration

comp->len = 0;
itemcount = 0;



loggingf (200, "value check, first pass \n");
loggingf (200, "local ftype\n");
for (int i = 0; i < local.procint; ++i)
	loggingf (200, "%d: %c\n", i, local_ftype [i]);

loggingf (200, "local fsize\n");
for (int i = 0; i < local.procint; ++i)
	loggingf (200, "%d: %d\n", i, local_sz [i]);


loggingf (200, "remote ftype\n");
for (int i = 0; i < remote.procint; ++i)
	loggingf (200, "%d: %c\n", i, remote_ftype [i]);

loggingf (200, "remote fsize\n");
for (int i = 0; i < remote.procint; ++i)
	loggingf (200, "%d: %d\n", i, remote_sz [i]);




// actual comparison loops
// do local against remote first
// then remote against local

/*
for (int loc = 0; loc < local.procint; ++loc)
{
for (int rem = 0; rem < remote.procint; ++rem)
{
if (remote_stat [rem] == 'c') continue;

//if pathlen ==
if (local_pathlen [loc] == remote_pathlen [rem])
{
midstr (local.p, temp1, local_start [loc], local_start [loc] + local_pathlen [loc]);
midstr (remote.p, temp2, remote_start [rem], remote_start [rem] + remote_pathlen [rem]);

// if paths match
if (!strcmp (temp1, temp2))
{ 
if (!remote_ftype [rem] == local_ftype [loc])
continue; // break if file types differ

++itemcount;

local_stat [loc] = 'c';
remote_stat [rem] = 'c';

if (local_mtime [loc] == remote_mtime [rem])
    continue;


if (local_mtime [loc] > remote_mtime [rem])
{
action = send;
rtn.bytesout += local_sz [loc];
rtn.filesout += 1;
entrylen = sprintf (temp2, "%s%s;%c;%d\n", action, temp1, local_ftype [loc], local_sz [loc]);
++itemcount;
comp->len += entrylen;
strcat (comp->p, temp2);

} // if local is newer

if (local_mtime [loc] < remote_mtime [rem])
{
action = get;
rtn.bytesin += remote_sz [rem];
rtn.filesin += 1;
entrylen = sprintf (temp2, "%s%s;%c;%d\n", action, temp1, remote_ftype [rem], remote_sz [rem]);
++itemcount;
comp->len += entrylen;
strcat (comp->p, temp2);

printf (temp2, "%s%s;%c;%d\n", action, temp1, remote_ftype [rem], remote_sz [rem]);


} // if remote is newer

//char local_ftype [local.procint];
//char remote_ftype [remote.procint];

//size_t local_sz [local.procint];
//size_t remote_sz [remote.procint];
break;    
}   // if !strcmp
} // if len ==
} // for remote
} // for local

// comparison list made, now loop through file lists for unchecked files
// check local files first
for (int i = 0; i < local.procint; ++i)
{
if (local_stat [i] == 'u')
{
action = send;
rtn.bytesout += local_sz [i];
rtn.filesout += 1;

dftype = getnext (local.p, ';', local_start [i], local.len);

midstr (local.p, temp1, local_start [i], dftype);

entrylen = sprintf (temp2, "%s%s;%c;%d\n", action, temp1, local_ftype [i], local_sz [i]);

comp->len += entrylen;
strcat (comp->p, temp2);
++itemcount;
} // file not yet added
} // for local list
*/


// refit for local
// check for unchecked local files
for (int i = 0; i < local.procint; ++i)
{
if (local_stat [i] == 'u')
{
action = send;
rtn.bytesout += local_sz [i];
rtn.filesout += 1;

//get and set path again
dftype = getnext (local.p, ';', local_start [i], local.len);
midstr (local.p, temp1, local_start [i], dftype);

//process entry string to be added
entrylen = sprintf (temp2, "%s%s;%c;%d\n", action, temp1, local_ftype [i], local_sz [i]);

loggingf (200, "entry-data: check unchecked local: \n%s%s;%c;%d\n", action, temp1, local_ftype [i], local_sz [i]);


comp->len += entrylen;
strcat (comp->p, temp2);

++itemcount;	
} // file not yet added

} // for local list
// refit for blocal



// check for unchecked remote files
for (int i = 0; i < remote.procint; ++i)
{
if (remote_stat [i] == 'u')
{
action = get;
rtn.bytesin += remote_sz [i];
rtn.filesin += 1;

//get and set path again
dftype = getnext (remote.p, ';', remote_start [i], remote.len);
midstr (remote.p, temp1, remote_start [i], dftype);

//process entry string to be added
entrylen = sprintf (temp2, "%s%s;%c;%d\n", action, temp1, remote_ftype [i], remote_sz [i]);

loggingf (200, "entry-data: check unchecked remote: \n%s%s;%c;%d\n", action, temp1, remote_ftype [i], remote_sz [i]);



comp->len += entrylen;
strcat (comp->p, temp2);

++itemcount;	
} // file not yet added

} // for remote list unchecked

loggingf (200, "value check, 2nd pass \n");
loggingf (200, "local ftype\n");
for (int i = 0; i < local.procint; ++i)
	loggingf (200, "%d: %c\n", i, local_ftype [i]);

loggingf (200, "local fsize\n");
for (int i = 0; i < local.procint; ++i)
	loggingf (200, "%d: %d\n", i, local_sz [i]);


loggingf (200, "remote ftype\n");
for (int i = 0; i < remote.procint; ++i)
	loggingf (200, "%d: %c\n", i, remote_ftype [i]);

loggingf (200, "remote fsize\n");
for (int i = 0; i < remote.procint; ++i)
	loggingf (200, "%d: %d\n", i, remote_sz [i]);



loggingf (200, "\n...\ncomp list\n%.*s\n...\n", comp->len, comp->p);
comp->procint = itemcount;
return (rtn);


} // make_comparison

int make_listfiles(struct buffer_data *list, const char *basepath)
{
DIR *dp;
struct dirent *ep;

const int debug = 0;


char filepath [string_sz] = "";
char subdir [string_sz] = "";
char fulldirpath [string_sz] = "";
char fullfilepath [string_sz] = "";


char entry [string_sz];

list->len = 0;
list->procint = 0;
int itemcount = 0;


while (1)
{

if (subdir[0] == 0)
{
    strcpy (fulldirpath, basepath);
}else{
    strcpy (fulldirpath, basepath);
    strcat (fulldirpath, "/");
    strcat (fulldirpath, subdir);  
} //if

if (debug)
    printf ("opening dir: %s\n", fulldirpath);
    
    dp = opendir (fulldirpath);
if (dp == NULL)
    {goto skipdir;}

while (ep = readdir (dp))
{
if (ep->d_name[0] ==  '.')
    continue;

if (subdir[0] == 0)
{
    strcpy (fullfilepath, basepath);
    strcat (fullfilepath, "/");
    strcat (fullfilepath, ep->d_name);
    
    strcpy (filepath, ep->d_name);
}else{
    strcpy (fullfilepath, basepath);
    strcat (fullfilepath, "/");
    strcat (fullfilepath, subdir);
    strcat (fullfilepath, "/");
    strcat (fullfilepath, ep->d_name);
    
    strcpy (filepath, subdir);
    strcat (filepath, "/");
    strcat (filepath, ep->d_name);
    
} // if

if (debug)
    printf ("full fp: %s: fp: %s\n", fullfilepath, filepath);


struct stat finfo;
stat (fullfilepath, &finfo);
char ftype;

if (S_ISDIR(finfo.st_mode)) // is dir
ftype = 'd';

if (S_ISREG(finfo.st_mode)) // is file
ftype = 'f';

int entrylen = sprintf (entry, "%s;%c;%d;%d\n", filepath, ftype, finfo.st_mtime, finfo.st_size);

if ((list->len + entrylen) > list->max)
    {printf ("buffer overflow, error: make_listfiles\n"); exit (0); }

strcat (list->p, entry);
++itemcount;

list->len += entrylen;

} // while dir entries
closedir (dp);

skipdir:
/////////////////
//inner loop
///////////////
while (1)
{
char ftype;
int d1, d2;

d1 = getnext (list->p, ';', list->procint, list->len);
if (d1 == -1)
{ list->procint = itemcount; return (itemcount); }

ftype = list->p[d1 + 1];

if (ftype == 'd')
{
midstr (list->p, subdir, list->procint, d1);

d2 = getnext (list->p, (char) 10, d1, list->len);
list->procint = d2 + 1;  

break;

//loop = 0;
    
} // if dir

d2 = getnext (list->p, (char) 10, d1, list->len);
list->procint = d2 + 1;  

} // while recursive dir search

} // outer loop

} // listfiles


