.PHONY: all clean
all: test_mylibc test_fs
	./test_fs
	./test_mylibc
test_fs: test_fs.o fs.o
	gcc -g -o test_fs test_fs.o fs.o
test_fs.o: test_fs.c fs.h
	gcc -g -c test_fs.c
fs.o: fs.c fs.h
	gcc -g -c fs.c
test_mylibc: test_mylibc.o mylibc.o fs.o
	gcc -g -o test_mylibc test_mylibc.o mylibc.o fs.o
test_mylibc.o: test_mylibc.c mylibc.h
	gcc -g -c test_mylibc.c
mylibc.o: mylibc.c mylibc.h
	gcc -g -c mylibc.c

clean:
	rm -f *.o test_fs fs_data test_mylibc
