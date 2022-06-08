// https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
// https://www.tutorialspoint.com/c_standard_library/c_אfunction_דfwrite.htm
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
    f->size = 9;
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
size_t myfread(void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream)
{
    int count = nmemb * size;
    char *buf = (char *)(malloc(sizeof(count + 1)));
    bzero(buf, count + 1);
    int i = 0;
    while (stream->p + i <= stream->size)
    {
        buf[i] = stream->data[stream->p + i];
        i++;
    }
    stream->p = stream->p + i;
    strncpy(ptr, buf, count);
    free(buf);
    return stream->p;
}

size_t myfwrite(const void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream)
{
    int count = size * nmemb;
    char *buf = (char *)ptr;
    int i = 0;
    for (i = 0; i < count; i++)
    {
        stream->data[stream->p + i] = buf[i];
    }
    stream->p += i;
    return stream->p;
}
int myfseek(myfile *stream, long offset, int whence)
{
    if (whence == SEEK_CUR)
    {
        stream->p += offset;
    }
    else if (whence == SEEK_END)
    {
        stream->p = stream->size + offset;
    }
    else if (whence == SEEK_SET)
    {
        stream->p = offset;
    }
    if (stream->p <= 0)
    {
        stream->p = 0;
    }
    return stream->p;
}
int myfscanf(void *restrict ptr, size_t size, size_t nmemb, myfile *restrict stream)
{
    myfread(ptr, size, nmemb, stream);
}
int myfprintf(int myfd, const void *buf, size_t count)
{
    mywrite(myfd, buf, count);
}