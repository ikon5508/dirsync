struct comp_data
{
int filesin;
int filesout;

size_t bytesin;
size_t bytesout;
};

struct comp_data make_comparison (struct buffer_data *comp, const struct buffer_data remote, const struct buffer_data local);

int make_listfiles(struct buffer_data *list, const char *basepath);

void process_complist (const struct buffer_data comp,const char *basepath, const int fd);

int filereciever (struct buffer_data *inbuff, const char *basepath, const int connfd);

extern const int debug;


