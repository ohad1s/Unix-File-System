// Credit: https://www.youtube.com/watch?v=n2AAhiujAqs&ab_channel=drdelhart
// Credit: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
#include "fs.h"

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;
struct open_file opened[12];
int find_empty_inode()
{
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        if (inodes[i].first_block == -1)
        {
            return i;
        }
    }
    return -1;
}
int find_empty_block()
{
    int i;
    for (i = 0; i < sb.num_blocks; i++)
    {
        if (dbs[i].next_block_num == -1)
        {
            return i;
        }
    }
    return -1;
}
int allocate_file(char name[8]){
    int in = find_empty_inode();
    int block = find_empty_block();
    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;

    strcpy((inodes[in]).name, name);
    // return the file descriptor
    return in;
}

int allocate_file_2(int size, const char* name) {
    if (strlen(name)>11) {
        perror("LONG_NAME");
        return -1;
    }
    int inode = find_empty_inode();
    if (inode == -1) {
        perror("ERROR_TO_FIND_EMPTY_INODE");
        return -1;
    }
    int curr_block = find_empty_block();
    if (curr_block == -1) {
        perror("ERROR_TO_FIND_EMPTY_BLOCK");
        return -1;
    }
    inodes[inode].size = size;
    inodes[inode].first_block = curr_block;
    dbs[curr_block].next_block_num = -2;
    strcpy(inodes[inode].name, name);
    if (size>BLOCK_SIZE) {
        int allocated_size = -(3*BLOCK_SIZE)/4;
        int next_block;
        while (allocated_size<size)
        {
            next_block = find_empty_block();
            if (next_block == -1) {
                perror("ERROR_TO_FIND_EMPTY_BLOCK");
                return -1;
            }
            dbs[curr_block].next_block_num = next_block;
            curr_block = next_block;
            allocated_size+=BLOCK_SIZE;
        }
    }
    dbs[curr_block].next_block_num = -2;

    return inode;
}

void shorten_file(int bn)
{
    int nn = dbs[bn].next_block_num;
    if (nn >= 0)
    {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
}
//######### me: ##############################
void mymfks(int size){
    create_fs(size);
}

int myopen(const char *pathname, int flags) {
    char str[100];
    strcpy(str, pathname);
    char *token;
    const char s[2] = "/";
    token = strtok(str, s);
    char curr_p[PATH_SIZE] = "";
    char last_p[PATH_SIZE] = "";
    while(token != NULL ) {
        if (token != NULL) {
            strcpy(last_p, curr_p);
            strcpy(curr_p, token);
        }
        token = strtok(NULL, s);
    }
    for (size_t i = 0; i < super_block.inodes; i++)
    {
        if (!strcmp(inodes[i].name, curr_p)) {
            if (inodes[i].dir!=2) {
                perror("DIR_NOT_FILE");
                return -1;
            }
            opened[i].fd = i;
            opened[i].pos = 0;
            return i;
        }
    }
//    ########### check from here ########################################### !!!! ###
//    int newfd = allocate_file_2(1, curr_p); ----- OK
//    int dirfd = myopendir(last_p); ------- FROM HERE!!!!
//    struct mydirent *currdir = myreaddir(dirfd);
//    currdir->fds[currdir->size++] = newfd;
//    return newfd;
//
//    int i = mycreatefile(lastpath, currpath);
//    opened[i].fd = i;
//    opened[i].pos = 0;
//    return i;
}
//############################################
void create_fs(int s)
{
    int size_without_superblock = s - sizeof(struct superblock);
    sb.num_inodes = (size_without_superblock/10)/(sizeof (struct inode));
    sb.num_blocks = (size_without_superblock-size_without_superblock/10)/(sizeof(struct disk_block));
    sb.size_blocks = sizeof(struct disk_block);
    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (i = 0; i < sb.num_inodes; i++)
    {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "no_name");
        inodes[i].if_dir=2; //2 for file, 1 for directory
    }
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_blocks; i++)
    {
        dbs[i].next_block_num = -1;
        strcpy(dbs[i].data, "_");
    }
}

int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags, const void *data)
{
    if (source==NULL && target==NULL){
        perror("Src and Trg are NULL");
        return -1;
    }
    if (source!=NULL){
        FILE *file;
        file = fopen(source, "r");
        fread(&sb, sizeof(struct superblock), 1, file);
        inodes = malloc(sizeof(struct inode) * sb.num_inodes);
        dbs = malloc(sizeof(struct disk_block)* sb.num_blocks);
        fread(inodes, sizeof(struct inode), sb.num_inodes, file);
        fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);
        fclose(file);
    }
    if (target!=NULL){
        sync_fs(target);
    }
}

void sync_fs(const char *target)
{
    FILE *file;
    file = fopen(target, "w+");
    fwrite(&sb, sizeof(struct superblock), 1, file);
    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    fclose(file);
}
int get_block_num(int file, int offset){
    int togo= offset;
    int bn = inodes[file].first_block;
    while (togo > 0)
    {
        bn = dbs[bn].next_block_num;
        togo--;
    }
    return bn;  
}

void set_filesize(int filenum, int size){
    int temp = size + BLOCKSIZE -1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    num--;
    while (num > 0)
    {
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2)
        {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[num].next_block_num;
        num--;
    }
    shorten_file(bn);
    dbs[bn].next_block_num = -2;
}

void write_byte(int filenum,int pos, char* data){
    int relative_block = pos / BLOCKSIZE;
    int bn = get_block_num(filenum, relative_block);
    int offset = pos % BLOCKSIZE;
    for (int i = 0; i < strlen(data); i++)
    {
        dbs[bn].data[offset +i] = data[i]; 
    }
    
    // dbs[bn].data[offset] = *data;
}


void print_fs()
{
    printf("SuperBlock Info:\n");
    printf("\tnum inodes %d\n", sb.num_inodes);
    printf("\tnum blocks %d\n", sb.num_blocks);
    printf("\tsize blocks %d\n", sb.size_blocks);
    printf("inodes\n");
    int i;
    for (i = 0; i < sb.num_inodes; i++)
    {
        printf("\tsize: %d block: %d name: %s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    }
    for (i = 0; i < sb.num_blocks; i++)
    {
        printf("\tblock_num: %d next block %d\n", i, dbs[i].next_block_num);
    }
}