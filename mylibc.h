
#include "fs.h"
#include <stdarg.h>
typedef struct myFILE
{
    char *name;
    char mode[3];
    int fd;
    int p;
} myFILE, *p_myFILE;

myFILE *myfopen(const char *pathname, const char *mode);
int myfclose(myFILE *stream);
size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream);
size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream);
int myfseek(myFILE *stream, long offset, int whence);
int myfscanf2(void *ptr, size_t size, size_t nmemb, myFILE *stream);
int myfprintf2(int myfd, const void *buf, size_t count);
int myfscanf(myFILE *stream, const char *format, ...);
int myfprintf(myFILE *stream, const char *format, ...);
