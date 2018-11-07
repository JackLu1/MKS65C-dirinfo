all: dirinfo.c
	gcc dirinfo.c -o ./dirinfo

run: all
	./dirinfo ${ARGS}
