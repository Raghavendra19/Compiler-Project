all: stage1exe

stage1exe: driver.c
	gcc -g driver.c -o stage1exe
clean:
	rm -rf *o stage1exe
