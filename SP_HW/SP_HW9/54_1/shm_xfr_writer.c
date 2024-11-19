/*
This writer reads from stdin and writes to the shared memory. 
Then the reader can read from the shared memory and display it through stdout 
Writer creates semaphores and shared memory so it has to be run first. 
Writer has access to the shared memory at the beginning. After that, reader will have access, and so on.
*/
#include "shm_xfr.h"

int
main(int argc, char *argv[])
{
	/* create semaphores */
	sem_t *writer_sem, *reader_sem;
	writer_sem = sem_open(WRITER_SEMAPHORE_NAME, O_CREAT|O_EXCL|O_RDWR, 0660, 1);
	if(writer_sem == SEM_FAILED)
	{
		perror("sem_open: writer_sem");
		exit(EXIT_FAILURE);
	}
	reader_sem = sem_open(READER_SEMAPHORE_NAME, O_CREAT|O_EXCL|O_RDWR, 0660, 0);
	if(reader_sem == SEM_FAILED)
	{
		perror("sem_open: reader_sem");
		exit(EXIT_FAILURE);
	}
	
	/* create a shared memory object */
	int shmdes = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, 0660);
	if(shmdes==-1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(shmdes, SHM_SIZE) == -1) {
	    perror("ftruncate");
	    exit(EXIT_FAILURE);
	}

	/* map the shared memory to virtual address space */
	char *shmem = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmdes, 0);
	if(shmem == (void *)-1)
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	/* now we can use the shared memory in ease */
	char input[SHM_SIZE];
	while(1)
	{
		sem_wait(writer_sem);

		if(fgets(input, SHM_SIZE, stdin)==NULL)
		{
			strcpy(shmem, "");
			sem_post(reader_sem);
			break;
		}
		strncpy(shmem, input, SHM_SIZE);

		sem_post(reader_sem);
	}
	sem_wait(writer_sem);	// wait for reader to let us have one more turn, so we know the reader has finished the last round

	/* Unmmap and Unlink shared memory and semaphores */
	if(munmap(shmem, SHM_SIZE)==-1)
	{
		perror("munmap");
	    exit(EXIT_FAILURE);
	}
    if (shm_unlink(SHM_NAME) == -1) 
    {
	    perror("shm_unlink");
	    exit(EXIT_FAILURE);
	}
	if (sem_unlink(WRITER_SEMAPHORE_NAME) == -1) 
	{
	    perror("sem_unlink writer semaphore failed");
	    exit(EXIT_FAILURE);
	}
	if (sem_unlink(READER_SEMAPHORE_NAME) == -1) 
	{
	    perror("sem_unlink reader semaphore failed");
	    exit(EXIT_FAILURE);
	}
    exit(EXIT_SUCCESS);
}