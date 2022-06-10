// https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
// https://www.tutorialspoint.com/c_standard_library/c_אfunction_דfwrite.htm
#include "mylibc.h"
#include "fs.h"

myFILE *myfopen(const char *pathname, const char *mode)
{
    int fd = myopen(pathname, -1);
    if (fd == -1)
    {
        perror("file not found\n");
        return NULL;
    }
    p_myFILE f = (p_myFILE)(malloc(sizeof(myFILE)));
    strcpy(f->mode, mode);
    f->fd = fd;
    f->p = 0;
    if (strcmp(mode, "a") == 0)
    {
        mylseek(f->fd, 0, SEEK_END);
    }
    f->name = name_disk;
    return f;
}
int myfclose(myFILE *stream)
{
    if (myclose(stream->fd) == 0)
    {
        free(stream);
        return 0; // 0 succsess -1 faild
    }
    free(stream);
    return -1; // 0 succsess -1 faild
}
size_t myfread(void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    return myread(stream->fd, ptr, nmemb * size);
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    int newPos = mywrite(stream->fd, ptr, size * nmemb);
    sync_fs(stream->name);
    return newPos;
}
int myfseek(myFILE *stream, long offset, int whence)
{
    mylseek(stream->fd, offset, whence);
    return 0;
}
int myfscanf2(void *ptr, size_t size, size_t nmemb, myFILE *stream)
{
    myfread(ptr, size, nmemb, stream);
}
int myfprintf2(int myfd, const void *buf, size_t count)
{
    mywrite(myfd, buf, count);
}

int myfscanf(myFILE *stream, const char *format, ...)
{
    va_list arguments;                     
    double sum = 0;
    /* Initializing arguments to store all values after num */
    va_start ( arguments, format );
    int len = strlen(format);
    int newPos = 0;
    for (size_t i = 0; i < len-1; i++)
    {
        if (format[i] == '%')
        {
            if (format[i+1] == 'd')
            {
                myfread(va_arg(arguments,void *), 1,sizeof(int),stream);
                newPos++;
            }
            if (format[i+1] == 'c')
            {
                myfread(va_arg(arguments,void *), 1,sizeof(char),stream);
                newPos++;
            }
        }
        return newPos;
    }
               
}
int myfprintf(myFILE *stream, const char *format, ...)
{
    va_list arguments;                     
    /* Initializing arguments to store all values after num */
    va_start ( arguments, format );
    int len = strlen(format);
    int newPos = 0;
    for (size_t i = 0; i < len-1; i++)
    {
        if (format[i] == '%')
        {
            if (format[i+1] == 'd')
            {

                myfwrite(va_arg(arguments,void *), 1,sizeof(int),stream);
                newPos++;
            }
            if (format[i+1] == 'c')
            {
                myfwrite(va_arg(arguments,void *), 1,sizeof(char),stream);
                newPos++;
            }
        }
        return newPos;
    }
}
