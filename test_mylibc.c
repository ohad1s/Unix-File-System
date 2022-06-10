#include "mylibc.h"

void Failed_test()
{
    printf("Test Failed! :(\n");
}

void Good_Test()
{
    printf("Test Passed! :)\n");
}

int main(int argc, char const *argv[])
{
    mymount("fs_data", NULL, NULL, 0, NULL);

    p_myFILE file_ptr1 = myfopen("root/dvir/d1", "r");
    // printf("fd: %d, %s\n" ,file_ptr1->fd,file_ptr1->mode);
    char buf1[4];
    myfread(buf1, 1, 4, file_ptr1);
    // printf("buf1 : %s\n", buf1);
    if (strncmp("dvir", buf1, 4) == 0)
    {
        Good_Test();
    }
    else{
        Failed_test();
    }
    myfclose(file_ptr1);

    file_ptr1 = myfopen("root/dvir/d1", "w");
    // printf("fd: %d, %s\n" ,file_ptr1->fd,file_ptr1->mode);
    myfseek(file_ptr1, 0, SEEK_SET);
    char* test12 = "dvir111111";
    myfwrite(test12,1, strlen(test12), file_ptr1);
    myfclose(file_ptr1);
    file_ptr1 = myfopen("root/dvir/d1", "r");
    char buf2[10];
    myfread(buf2, 1, strlen(test12), file_ptr1);
    // printf("buf2 : %s\n", buf2);
    if (strcmp("dvir111111",buf2) == 0)
    {
        Good_Test();
    }
    else{
        Failed_test();
    }
    myfclose(file_ptr1);
    return 0;
}