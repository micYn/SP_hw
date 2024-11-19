#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
void errorHandler(char *str){
	perror(str);
	exit(EXIT_FAILURE);
}
int 
main(int argc, char *argv[]){

	if(argc != 3 || strcmp(argv[1], "--help")==0){
		fprintf(stderr, "Usage: %s <src filename> <dest filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int srcFd, destFd;
	char *addr1, *addr2;
	struct stat statbuf;
	srcFd = open(argv[1], O_RDONLY);	// open src file in read-only mode
	if(srcFd == -1)
		errorHandler("open");

	if(fstat(srcFd, &statbuf)==-1)		// get the size of the src file
		errorHandler("fstat");

	// create a private file mapping between this process and the src file:
	addr1 = mmap(NULL, (size_t)statbuf.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
	if(addr1==(void *)-1)
		errorHandler("mmap");
	if(close(srcFd)==-1)
		errorHandler("close");

	// open dest file and truncate it to make its size equal to the size of src file:
	destFd = open(argv[2], O_CREAT | O_RDWR, 0664);
	if(destFd==-1)
		errorHandler("open");

	if(ftruncate(destFd, statbuf.st_size)==-1)
		errorHandler("ftruncate");

	// create a shared file mapping between this process and the dest file
	// so that we can modify the content of dest file:
	addr2 = mmap(NULL, (size_t)statbuf.st_size, PROT_WRITE, MAP_SHARED, destFd, 0);
	if(addr2==(void *)-1)
		errorHandler("mmap");
	if(close(destFd)==-1)
		errorHandler("close");

	// memcpy the content from addr1 to addr2, and this will copy the src file to dest file
	memcpy(addr2, addr1, (size_t)statbuf.st_size);

	exit(EXIT_SUCCESS);
}