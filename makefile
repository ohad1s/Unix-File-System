.PHONY: all clean
all: test

test: test.o fs.o
	gcc -g -o test test.o fs.o
test.o: test.c fs.h
	gcc -g -c test.c
fs.o: fs.c fs.h
	gcc -g -c fs.c

clean:
	rm -f *.o test fs_data testfile.txt
    
