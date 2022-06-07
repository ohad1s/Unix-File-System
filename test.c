#include "fs.h"

int main(int argc, char const *argv[])
{
    mymfks(10000);
    // mount_fs();
    // allocate_file("first");

    // set_filesize(0, 1000);
    // print_fs();

    // set_filesize(0, 5000);
    // print_fs();

    // set_filesize(0, 100);
    // print_fs();
    int file = allocate_file("first");
    // set_filesize(file, 5000);
    char* data = "dvir";
    
    int i;
    // for (i = 0; i < 5; i++)
    // {
        write_byte(file, 0, data);
    // }
    // create_fs();
    // sync_fs();
    // mount_fs();
    // allocate_file( " first " );
    sync_fs();
    
    print_fs();
    printf("done\n");

    return 0;
}
