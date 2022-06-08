#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BLOCKSIZE 512
#define PATH_SIZE 12

struct superblock {
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

struct inode {
    int size;
    int first_block;
    int if_dir;
    char name[8];
};

struct disk_block {
    int next_block_num;
    char data[BLOCKSIZE];
};

struct open_file {
    int fd;
    int pos;
};

struct mydirent {
    int size;
    int fds[12];
    char name[PATH_SIZE];
};


void mymfks(int size);
int myclosedir(int myfd);
void create_fs(int s);
int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data);
void sync_fs(const char* target);
int myclose(int fd);
int allocate_file(int size, const char *name);
void set_filesize(int filenum, int size);
void write_byte(int filenum,int pos, char* data);
int get_block_num(int file, int offset);
void shorten_file(int bn);
void print_fs();
int myopendir(const char *name);
struct mydirent *myreaddir(int dirp);
int myopen(const char *pathname, int flags);
size_t mywrite(int myfd, const void *buf, size_t count);
int mylseek(int myfd, int offset, int whence);
size_t myread(int myfd, void *buf, size_t count);

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;
struct open_file opened[12];