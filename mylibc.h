
#include "fs.h"

typedef struct myfile{
    char *data;
    char mode[3];
    int fd;
    int size;
    int p;
}myfile, *p_myfile;


myfile *myfopen(const char *restrict pathname, const char *restrict mode);
int myfclose(myfile *stream);
size_t myfread(void *restrict ptr, size_t size, size_t nmemb,myfile *restrict stream);
size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream);
int myfseek(myfile *stream, long offset, int whence);
int myfscanf(void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream);
int myfprintf(int myfd, const void *buf, size_t count);