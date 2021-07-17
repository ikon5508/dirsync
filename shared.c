#include "libmemory.h"
#include "socket.h"
#include "shared.h"
#include "logging.h"



void process_complist (const struct buffer_data comp, const char *basepath, const int fd)
{
const int debug = 100;


if (debug > 99) printf ("now process comparison list\n%s\n", comp.p);


int start_line = 0;
int end_line = 0;
	
char entry [string_sz];
char path [string_sz];
char fullpath [string_sz];
	
char action;

int itemcount = 0;

while (itemcount < comp.procint)
{
start_line = end_line + 1;
end_line = getnext (comp.p, (char) 10, start_line, comp.len);

midstr (comp.p, path, start_line, end_line);

printf ("%d: %s\n", itemcount, path);



	
++itemcount;	
}	
}




void make_comparison (struct buffer_data *comp, const struct buffer_data remote, const struct buffer_data local)
{
const int debug = 0;

// 1 for basic, 200 for local list,, 100 for matching info 

if (debug > 199)
    printf ("%s\n", remote.p);

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

size_t bytesin = 0;
size_t bytesout = 0;
int filesin = 0;
int filesout = 0;


//char temp [string_sz];
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
end_line = getnext (remote.p, (char) 10, 0, remote.len);
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
// actual comparison loops
for (int loc = 0; loc < local.procint; ++loc)
{
//if (local_stat [loc] == 'c') continue;

if (debug > 99) {
    midstr (local.p, temp1, local_start [loc], local_start [loc] + local_pathlen [loc]);
    printf ("loc: %d: %s\n", loc, temp1);}
    
for (int rem = 0; rem < remote.procint; ++rem)
{
if (remote_stat [rem] == 'c') continue;

if (debug > 99) {
    midstr (remote.p, temp2, remote_start [rem], remote_start [rem] + remote_pathlen [rem]);
    printf ("rem: %d: %s\n", rem, temp2);}

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
	//if (debug) printf ("%d: %s matched!!!\n", itemcount, temp1);

	local_stat [loc] = 'c';
	remote_stat [rem] = 'c';

if (local_mtime [loc] == remote_mtime [rem])
{    
	if (debug) printf ("items same in age, no action taken\n");
	continue;
} // if mtime is same

if (local_mtime [loc] > remote_mtime [rem])
{
	if (debug) printf ("local is newer\n");
	action = send;
	bytesout += local_sz [loc];
	filesout += 1;
	entrylen = sprintf (temp2, "%s%s\n", action, temp1);

comp->len += entrylen;
strcat (comp->p, temp2);

} // if local is newer

if (local_mtime [loc] < remote_mtime [rem])
{
	if (debug) printf ("remote is newer\n");
	action = get;
	bytesin += remote_sz [rem];
	filesin += 1;
	entrylen = sprintf (temp2, "%s%s\n", action, temp1);

comp->len += entrylen;
strcat (comp->p, temp2);

} // if remote is newer




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
	bytesout += local_sz [i];
	filesout += 1;

dftype = getnext (local.p, ';', local_start [i], local.len);

midstr (local.p, temp1, local_start [i], dftype);

entrylen = sprintf (temp2, "%s%s\n", action, temp1);

comp->len += entrylen;
strcat (comp->p, temp2);

	
} // file not yet added
} // for local list

// check for unchecked remote files
for (int i = 0; i < remote.procint; ++i)
{
if (remote_stat [i] == 'u')
{
	
	action = get;
	bytesin += remote_sz [i];
	filesin += 1;

dftype = getnext (remote.p, ';', remote_start [i], remote.len);

midstr (remote.p, temp1, remote_start [i], dftype);

entrylen = sprintf (temp2, "%s%s\n", action, temp1);

comp->len += entrylen;
strcat (comp->p, temp2);

	
} // file not yet added

} // for remote list


comp->procint = filesin + filesout;

//entrylen = sprintf (temp1, "files in: %d:%d bytes, files out: %d:%d bytes\n", filesin, bytesin, filesout, bytesout);
//strcat (comp->p, temp1);
//comp->len += entrylen;

if (debug) printf ("%s\n", comp->p);


if (debug) printf ("local item count is: %d len: %d\n", local.procint, local.len);

if (debug) printf ("remote item count is: %d len: %d\n", remote.procint, remote.len);

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


if (debug)
    printf ("len: %d, %s\n", list->len, entry);


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

//{loop = 0; break;}
    
    
ftype = list->p[d1 + 1];


if (ftype == 'd')
{
midstr (list->p, subdir, list->procint, d1);


//printf ("proc: %d, d1: %d, file name: %s, type: %c\n", list->procint, d1, path, ftype);


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


