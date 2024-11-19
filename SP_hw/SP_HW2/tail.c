/*
Usage: ./tail [-n num] file   
(default num is 10)
    1. open the file and read it from the back chunk by chunk (we use O_DSYNC|O_RSYNC to ensure all the "needed" changes are transferred to the disk before we read)
    2. count the number of '\n' and get the position where the last num lines start.
    3. read from there all the way to EOF and print it on the terminal. (kernel will do some kernel buffering then let them appear on the terminal)
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#define BUF_SIZE 1024

void errorHandling(char *str){
    perror(str);
    exit(EXIT_FAILURE);
}
int 
main(int argc, char *argv[]){
    if( (argc!=2 && argc!=4) || strcmp(argv[1],"--help")==0){
        fprintf(stderr, "Usage: %s [-n num] file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd;
    off_t position;
    size_t bytesToRead;
    ssize_t bytesRead;
    int newlineCount = 0, newlineNeeded = (argc==4 ? atoi(argv[2]):10);
    // printf("%d",newlineNeeded);
    char buf[BUF_SIZE];

    if(argc==2){
        fd = open(argv[1],O_RDONLY|O_DSYNC|O_RSYNC);
    }
    else{
        fd = open(argv[3],O_RDONLY|O_DSYNC|O_RSYNC);
    }
    if(fd==-1){
        errorHandling("open");
    }

    
    position = lseek(fd,0,SEEK_END);    // position has the size of the file and the file offset is set to the next byte of the last byte.
    if(position==-1){    
        errorHandling("lseek");
    }
       
    while(1){
        bytesToRead = (position<BUF_SIZE) ? position:BUF_SIZE;  //bytesToRead will be position when BUF_SIZE is too big for the unread part of the file

        position-=bytesToRead;  //now position will give us the offset that we should start reading with

        if(lseek(fd, position, SEEK_SET)==-1){    
            errorHandling("lseek");
        }
        memset(buf, 0, BUF_SIZE);
        if(read(fd, buf, bytesToRead)==-1){  
            errorHandling("read");
        }

        int i,flag=0;   //don't ever use an unsigned integer as your loop iterator. cuz it never goes negative
        for(i=bytesToRead-1;i>=0;i--){
            if(buf[i]=='\n'){
                newlineCount++;
            }
            if(newlineCount==newlineNeeded+1){
                position+=(i+1);    //the last newline character is needless
                flag=1;
                break;
            }
        }

        if(position==0 || flag==1){     // Either we get enough newlines or the file is fully read, we break. For both cases, position now holds the desired offset.
            if(lseek(fd,position,SEEK_SET)==-1){
                errorHandling("lseek");
            }
            break;
        }
    }

    memset(buf, 0, BUF_SIZE);
    while( (bytesRead=read(fd,buf,BUF_SIZE))>0 ){
        if(write(STDOUT_FILENO,buf,bytesRead)==-1){
            errorHandling("write");
        }
    }
    if(bytesRead==-1){
        errorHandling("read");
    }

    exit(EXIT_SUCCESS);
}
