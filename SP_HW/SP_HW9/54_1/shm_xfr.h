#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define WRITER_SEMAPHORE_NAME "/mywritersemaphore_B103040009"
#define READER_SEMAPHORE_NAME "/myreadersemaphore_B103040009"

#define SHM_NAME "/mysharedmemory_B103040009"
#define SHM_SIZE 256
