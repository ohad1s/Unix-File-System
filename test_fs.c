#include "fs.h"

void Failed_test() {
    printf("Test Failed! :(\n");
}

void Good_Test() {
    printf("Test Passed! :)\n");
}

int main(int argc, char const *argv[]) {

    mymfks(10000);
    int t1 = myopendir("root/ohad");
    int t2 = myopendir("root/dvir");
    if (t1 == t2) {
        Failed_test();
    } else {
        Good_Test();
    }

    int fd1 = myopen("root/ohad/o1", 0);
    int fd2 = myopen("root/ohad/o1", 0);

    if (fd1 == fd2) {
        Good_Test();
    } else {
        Failed_test();
    }
    int fd3 = myopen("root/dvir/d1", 0);
    if (fd1 == fd3) {
        Failed_test();
    } else {
        Good_Test();
    }

    fd1 = myclose(fd1);

    if (fd1 == fd2) {
        Failed_test();
    } else {
        Good_Test();
    }

    char *test1 = " ohad_test";

    int ptr2 = mywrite(fd2, test1, 12);
    char ohad_buf[12];
    mylseek(fd2, -13, SEEK_CUR);
    myread(fd2, ohad_buf, 12);
    if (strcmp(ohad_buf, "ohad_test") == 0) {
        Good_Test();
    } else {
        Failed_test();
    }


    char *test2 = "dvir_test";

    int ptr = mywrite(fd3, test2, 12);
    char dvir_buf[12];
    mylseek(fd3, 0, SEEK_SET);
    myread(fd2, dvir_buf, 12);

    if (strcmp(ohad_buf, dvir_buf) == 0) {
        Failed_test();
    } else {
        Good_Test();
    }

    char *ohad_buf2 = malloc(4);
    mylseek(fd2, -9, SEEK_END);
    myread(fd2, ohad_buf2, 4);

    if (strcmp(ohad_buf, ohad_buf2) == 0) {
        Failed_test();
    } else {
        Good_Test();
    }

    if (strcmp("ohad", ohad_buf2) == 0) {
        Good_Test();
    } else {
        Failed_test();
    }

    mymount(NULL, "fs_data", NULL, 0, NULL);

    return 0;
}