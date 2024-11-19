#include "shm_xfr.h"

int
main(int argc, char *argv[])
{
	/* semaphores and shared memory object are all supposed to be already created by the writer
	   so the reader can just open them and start using them */

	/* open semaphores */
	sem_t *writer_sem, *reader_sem;
	writer_sem = sem_open(WRITER_SEMAPHORE_NAME, O_RDWR);
	reader_sem = sem_open(READER_SEMAPHORE_NAME, O_RDWR);
	if(writer_sem==SEM_FAILED)
	{
		perror("sem_open: writer_sem");
		exit(EXIT_FAILURE);
	}
	if(reader_sem==SEM_FAILED)
	{
		perror("sem_open: reader_sem");
		exit(EXIT_FAILURE);
	}

	/* open shared memory and do mapping */
	int shmdes = shm_open(SHM_NAME, O_RDWR, 0);
	if(shmdes==-1)
	{
		perror("shmdes");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(shmdes, SHM_SIZE) == -1) {
	    perror("ftruncate");
	    exit(EXIT_FAILURE);
	}

	char *shmem = mmap(NULL, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, shmdes, 0);
	if(shmem == (void *)-1)
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		sem_wait(reader_sem);
		if(strlen(shmem)==0)
		{
			sem_post(writer_sem);
			break;
		}
		printf("%s", shmem);
		sem_post(writer_sem);
	}

	/* Unmmap and Unlink shared memory and semaphores */
	munmap(shmem, SHM_SIZE);
    // shm_unlink(SHM_NAME);

    exit(EXIT_SUCCESS);
}