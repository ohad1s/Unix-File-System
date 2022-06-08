// Credit: https://www.youtube.com/watch?v=n2AAhiujAqs&ab_channel=drdelhart
// Credit: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
#include "fs.h"

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;
struct open_file opened[12];

int find_empty_inode() {
    int i;
    for (i = 0; i < sb.num_inodes; i++) {
        if (inodes[i].first_block == -1) {
            return i;
        }
    }
    return -1;
}

int find_empty_block() {
    int i;
    for (i = 0; i < sb.num_blocks; i++) {
        if (dbs[i].next_block_num == -1) {
            return i;
        }
    }
    return -1;
}

int allocate_file(int size, const char *name) {
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
    return inode;
}

void shorten_file(int bn) {
    int nn = dbs[bn].next_block_num;
    if (nn >= 0) {
        shorten_file(nn);
    }
    dbs[bn].next_block_num = -1;
}

void mymfks(int size) {
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
    while (token != NULL) {
        strcpy(last_p, curr_p);
        strcpy(curr_p, token);
        token = strtok(NULL, s);
    }
    for (size_t i = 0; i < sb.num_inodes; i++) {
        if (!strcmp(inodes[i].name, curr_p)) {
            if (inodes[i].if_dir != 2) {
                perror("DIR_NOT_FILE");
                return -1;
            }
            opened[i].fd = i;
            opened[i].pos = 0;
            return i;
        }
    }
    int newfd = allocate_file(1, curr_p);
    int dirfd = myopendir(last_p);
    struct mydirent *curr = myreaddir(dirfd);
    curr->fds[curr->size++] = newfd;
    opened[newfd].fd = newfd;
    opened[newfd].pos = 0;
    return newfd;
}

struct mydirent *myreaddir(int dirp) {
    if (inodes[dirp].if_dir != 1) {
        return NULL;
    }
    return (struct mydirent *) dbs[inodes[dirp].first_block].data;
}

int myopendir(const char *name) {
    char str[100];
    strcpy(str, name);
    char *token;
    const char s[2] = "/";
    token = strtok(str, s);
    char curr_p[PATH_SIZE] = "";
    char last_p[PATH_SIZE] = "";
    while (token != NULL) {
        strcpy(last_p, curr_p);
        strcpy(curr_p, token);
        token = strtok(NULL, s);
    }
    for (size_t i = 0; i < sb.num_inodes; i++) {
        if (!strcmp(inodes[i].name, curr_p)) {
            if (inodes[i].if_dir != 1) {
                perror("DIR_NOT_FILE");
                return -1;
            }
            return i;
        }
    }
    int my_fd = myopendir(last_p);
    if (my_fd == -1) {
        perror("ERROR");
        return -1;
    }
    if (inodes[my_fd].if_dir != 1) {
        perror("DIR_NOT_FILE");
        return -1;
    }
    int d_b = inodes[my_fd].first_block;
    struct mydirent *currdir = (struct mydirent *) dbs[d_b].data;
    int newdirfd = allocate_file(sizeof(struct mydirent), curr_p);
    currdir->fds[currdir->size++] = newdirfd;
    inodes[newdirfd].if_dir = 1;
    struct mydirent *newdir = malloc(sizeof(struct mydirent));
    newdir->size = 0;
    for (size_t i = 0; i < 12; i++) {
        newdir->fds[i] = -1;
    }

    char *new_dir2 = (char *) newdir;
    write_byte(newdirfd, 0, new_dir2);
    opened[my_fd].pos += (sizeof(struct mydirent));
    strcpy(newdir->name, name);
    return newdirfd;
}


void create_fs(int s) {
    int size_without_superblock = s - sizeof(struct superblock);
    sb.num_inodes = (size_without_superblock / 10) / (sizeof(struct inode));
    sb.num_blocks = (size_without_superblock - size_without_superblock / 10) / (sizeof(struct disk_block));
    sb.size_blocks = sizeof(struct disk_block);
    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (i = 0; i < sb.num_inodes; i++) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");
        inodes[i].if_dir = 2; //2 for file, 1 for directory
    }
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for (i = 0; i < sb.num_blocks; i++) {
        dbs[i].next_block_num = -1;
        strcpy(dbs[i].data, "_");
    }
    int myfd = allocate_file(sizeof(struct mydirent), "root");
    inodes[myfd].if_dir = 1;
    struct mydirent *rootdir = malloc(sizeof(struct mydirent));
    for (size_t i = 0; i < 12; i++) {
        rootdir->fds[i] = -1;
    }
    strcpy(rootdir->name, "root");
    rootdir->size = 0;
    char *rootdiraschar = (char *) rootdir;
    write_byte(myfd, 0, rootdiraschar);
    opened[myfd].pos += (sizeof(rootdiraschar));
    free(rootdir);
}

int mymount(const char *source, const char *target, const char *filesystemtype, unsigned long mountflags,
            const void *data) {
    if (source == NULL && target == NULL) {
        perror("Src and Trg are NULL");
        return -1;
    }
    if (source != NULL) {
        FILE *file;
        file = fopen(source, "r");
        fread(&sb, sizeof(struct superblock), 1, file);
        inodes = malloc(sizeof(struct inode) * sb.num_inodes);
        dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
        fread(inodes, sizeof(struct inode), sb.num_inodes, file);
        fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);
        fclose(file);
    }
    if (target != NULL) {
        sync_fs(target);
    }
}

void sync_fs(const char *target) {
    FILE *file;
    file = fopen(target, "w+");
    fwrite(&sb, sizeof(struct superblock), 1, file);
    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);
    fclose(file);
}

int get_block_num(int file, int offset) {
    int togo = offset;
    int bn = inodes[file].first_block;
    while (togo > 0) {
        bn = dbs[bn].next_block_num;
        togo--;
    }
    return bn;
}

void set_filesize(int filenum, int size) {
    int temp = size + BLOCKSIZE - 1;
    int num = temp / BLOCKSIZE;
    int bn = inodes[filenum].first_block;
    num--;
    while (num > 0) {
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2) {
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

void write_byte(int filenum, int pos, char *data) {
    int relative_block = pos / BLOCKSIZE;
    int bn = get_block_num(filenum, relative_block);
    int offset = pos % BLOCKSIZE;
    for (int i = 0; i < strlen(data); i++) {
        dbs[bn].data[offset + i] = data[i];
    }
}

int myclose(int fd) {
    opened[fd].fd = -1;
    opened[fd].pos = -1;
}

int myclosedir(int myfd) {
    return 0;
}

void print_fs() {
    printf("SuperBlock Info:\n");
    printf("\tnum inodes %d\n", sb.num_inodes);
    printf("\tnum blocks %d\n", sb.num_blocks);
    printf("\tsize blocks %d\n", sb.size_blocks);
    printf("inodes\n");
    int i;
    for (i = 0; i < sb.num_inodes; i++) {
        printf("\tsize: %d block: %d name: %s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    }
    for (i = 0; i < sb.num_blocks; i++) {
        printf("\tblock_num: %d next block %d\n", i, dbs[i].next_block_num);
    }
}

size_t myread(int myfd, void *buf, size_t count) {
    if (inodes[myfd].if_dir == 1) {
        perror("DIR_NOT_FILE");
        return -1;
    }
    char *buffer = malloc(count + 1);
    for (size_t i = 0; i < count; i++) {


        int rb = inodes[myfd].first_block;
        int pos = opened[myfd].pos;
        while (pos >= BLOCKSIZE) {
            pos -= BLOCKSIZE;
            rb = dbs[rb].next_block_num;
            if (rb == -1 || rb == -2) {
                return -1;
            }
        }
        buffer[i] = dbs[rb].data[pos];
        opened[myfd].pos += 1;
    }
    buffer[count] = '\0';
    strncpy(buf, buffer, count);
    free(buffer);
    return opened[myfd].pos;
}


int mylseek(int myfd, int offset, int whence) {
    if (whence == SEEK_CUR) {
        opened[myfd].pos += offset;
    } else if (whence == SEEK_END) {
        opened[myfd].pos = inodes[myfd].size + offset;
    } else if (whence == SEEK_SET) {
        opened[myfd].pos = offset;
    }
    if (opened[myfd].pos < 0) {
        opened[myfd].pos = 0;
    }
    return opened[myfd].pos;
}

size_t mywrite(int myfd, const void *buf, size_t count) {
    if (inodes[myfd].if_dir == 1) {
        perror("DIR_NOT_FILE");
        return -1;
    }
    char *buffer = (char *) buf;
    write_byte(myfd, opened[myfd].pos, buffer);
    opened[myfd].pos += (count);
    return opened[myfd].pos;
}
