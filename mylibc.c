// https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
#include "mylibc.h"
#include "fs.h"

myfile *myfopen(const char *restrict pathname, const char *restrict mode)
{
    int fd = myopen(pathname, -1);
    if (fd == -1)
    {
        perror("file not found\n");
        return NULL;
    }
    p_myfile f = (p_myfile)(malloc(sizeof(myfile)));
    strcpy(f->mode, mode);
    f->fd = fd;
    f->p = 0;
    if (strcmp(mode, "a") == 0)
    {
        f->p = inodes[fd].size;
    }
    f->size = inodes[fd].size;
    f->data = (char *)(malloc(sizeof(f->size)));
    if (strcmp(mode, "w") == 0)
    {
        bzero(f->data, f->size);
    }
    if (strcmp(mode, "r") == 0)
    {
        myread(fd, f->data, f->size);
    }
    myclose(fd);
    return f;
}
int myfclose(myfile *stream)
{
    if (strcmp(stream->mode, "r"))
    {
        free(stream->data);
        free(stream);
        return 0; // 0 succsess -1 faild
    }
    write_byte(stream->fd, 0, stream->data);
    stream->p += stream->size;
    free(stream->data);
    free(stream);
    return 0; // 0 succsess -1 faild
}
size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream){
    int count = nmemb*size;
    char* buf = (char*)(malloc(sizeof(count +1)));
    bzero(buf,count+1);
}
size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream);
int myfseek(myfile *stream, long offset, int whence);
int myfscanf(myfile *restrict stream,
             const char *restrict format, ...);
int myfprintf(myfile *restrict stream,
              const char *restrict format, ...);
