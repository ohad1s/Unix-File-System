#include "fs.h"

void red() {
    printf("\033[1;31m");
}

void green() {
    printf("\033[1;32m");
}

void white() {
    printf("\033[1;0m");
}

int main(int argc, char const *argv[]) {

    printf("creating root..\n");
    mymfks(10000);
//    mymount(NULL, "file", NULL, 0, NULL);
//    // ############### test1: open the same directory ###############
    int t1 = myopendir("root/test1");
    int t2 = myopendir("root/test1");
    int t = (t1 == t2);
    if(t == 1)
    {
        green();
        printf("myopendir test PASSED\n");
    }else{
        red();
        printf("myopendir test FAILED\n");
    }
    myopendir("root/test2");

    // ############### test2: open the same file with myopen ###############


    int fd = myopen("root/test1/file1", 0);
    int testfd = myopen("root/test1/file1", 0);
    int fd2 = myopen("root/test2/file2",0);
    int fd3 = myopen("root/test1/file2",0);
    int i = (fd == testfd);
    if(i == 1)
    {
        green();
        printf("myopen test PASSED\n");
    }else{
        red();
        printf("myopen test FAILED\n");
    }
    white();
    // ############### test3: open the same file in different directories ###############

    i = (fd2 == fd3);
    if(i == 1)
    {
        green();
        printf("different directory open file PASSED\n");
    }else{
        red();
        printf("different directory open file FAILED\n");
    }
    white();


    // ############### test4: closing file changing fd ###############

    testfd = myclose(testfd);

    i = (fd != testfd);
    if(i == 1)
    {
        green();
        printf("myclose test PASSED\n");
    }else{
        red();
        printf("myclose test FAILED\n");
    }
    white();
    // ############### test5: write, read and seek operation on files ###############

    char* test = "this is a test\n";

    int ptr =  mywrite(fd2, test, 25);
    char buf[25];
    char buf2[25];
    mylseek(fd2, -25, SEEK_CUR);
    myread(fd2, buf, 25);


    if(strcmp(buf, test) ==0)
    {
        green();
        printf("read test PASSED\n");
    }else
    {
        red();
        printf("read test FAILED\n");
    }
    white();
    mylseek(fd2,-15,SEEK_CUR);
    myread(fd2, buf2, 15);

    if(strcmp(buf2, "test\n") ==0)
    {
        green();
        printf("seek test PASSED\n\n");
    }else
    {
        red();
        printf("seek test FAILED\n\n");
    }
    white();
    // ############### test6: testing write big file ###############

    char* test2 = "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$test";
    int newfd = myopen("root/test1/bigfile", 0);
    char *bigbuf = malloc(8);
    mywrite(newfd,test2,strlen(test2));

    mylseek(newfd,-8,SEEK_CUR);
    myread(newfd,bigbuf,8);
    printf("bigbuf is: %s",bigbuf);
    if(strcmp(bigbuf, "$$$$test") == 0)
    {
        green();
        printf("seek test big file PASSED\n\n");
    }else
    {
        red();
        printf("seek test big file FAILED\n\n");
    }
    white();

    printf("our fs looks like this:\n\n");
//    printdir("root");
//    printdir("test1");
//    printdir("test2");

    mymount(NULL, "testfile.txt", NULL,0,NULL);

    return 0;
}