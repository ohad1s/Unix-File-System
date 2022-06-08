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

    p_myfile file_ptr1 = myfopen("root/dvir/d1", "r");
    if (strncmp(file_ptr1->data, "dvir_test", 9) == 0)
    {
        Good_Test();
    }
    else
    {
        Failed_test();
    }

    char buf[5];
    myfread(buf, 1, 4, file_ptr1);
    if (strcmp("dvir", buf) == 0)
    {
        Good_Test();
    }
    else
    {
        Failed_test();
    }
    myfclose(file_ptr1);
    p_myfile file_ptr2 = myfopen("root/ohad/o1", "w+");
    char * test_ohad = "Ciii";
    myfwrite(test_ohad,1,5,file_ptr2);
    bzero(buf, 5);
    myfread(buf,1,5,file_ptr2);
    if (strcmp("Ciii", file_ptr2->data) == 0)
    {
        Good_Test();
    }
    else
    {
        Failed_test();
    }
    bzero(buf,5);
    myfseek(file_ptr2, 2 , SEEK_SET);
    myfread(buf,1,2,file_ptr2);
    if (strcmp("ii", buf) == 0)
    {
        Good_Test();
    }
    else
    {
        Failed_test();
    }
    return 0;
}