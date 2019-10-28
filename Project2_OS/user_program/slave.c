#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE) * 4
#define BUF_SIZE 512

int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i, dev_fd, file_fd;// the fd for the device and the fd for the input file
	size_t ret, file_size = 0, data_size = -1, page_size = 0;
	char file_name[50];
	char method[20];
	char ip[20];
	struct timeval start;
	struct timeval end;
	double trans_time; //calulate the time between the device is opened and it is closed
	char *kernel_address, *file_address;
	char* mmapped;


	strcpy(file_name, argv[1]);
	strcpy(method, argv[2]);
	strcpy(ip, argv[3]);
	
	if( (dev_fd = open("/dev/slave_device", O_RDWR)) < 0)//should be O_RDWR for PROT_WRITE when mmap()
	{
		perror("failed to open /dev/slave_device\n");
		return 1;
	}
	
	gettimeofday(&start ,NULL);
	
	if( (file_fd = open (file_name, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600)) < 0)
	{
		perror("failed to open input file\n");
		return 1;
	}
	
	if(ioctl(dev_fd, 0x12345677, ip) == -1)	//0x12345677 : connect to master in the device
	{
		perror("ioclt create slave socket error\n");
		return 1;
	}

	switch(method[0])
	{
		
		
		case 'f'://fcntl : read()/write()
			do
			{
				ret = read(dev_fd, buf, sizeof(buf)); // read from the the device
				write(file_fd, buf, ret); //write to the input file
				file_size += ret;
			}while(ret > 0);
			break;
		
		case 'm': 
			
			while((ret = read(dev_fd, buf, sizeof(buf))) > 0){
				size_t remaining = 0, i;
				
				if(data_size == -1 || (long)PAGE_SIZE - (long)data_size - (long)ret < 0){
					if(data_size != -1){

						remaining = PAGE_SIZE - data_size;
						page_size += PAGE_SIZE;
						
						for(i = 0; i < remaining; ++i)
							mmapped[data_size + i] = buf[i];
						if(msync(mmapped, PAGE_SIZE, MS_ASYNC) == -1)  
							perror("Disk sync error\n");
						if(munmap(mmapped, PAGE_SIZE) == -1)
							perror("Map file error\n");										
					}
					if(lseek(file_fd, PAGE_SIZE-1, SEEK_END) == -1)
						perror("File cannot be stretched\n");
					if(write(file_fd, "", 1) == -1)
						perror("File cannot be resized\n");	
					
					mmapped = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, page_size);
					if(mmapped == MAP_FAILED)
						perror("Failed mmap!\n");

					data_size = 0;
				}
				
				for(i = 0; i < ret - remaining; i++)
					mmapped[data_size + i] = buf[remaining + i];

				data_size += ret, file_size += ret;
			}
			
			if(msync(mmapped, PAGE_SIZE, MS_ASYNC) == -1)
				perror("Disk cannot be synced\n");
			if(munmap(mmapped, PAGE_SIZE) == -1)
				perror("File could not be unmapped\n");
			if(ftruncate(file_fd, file_size) == -1)
				perror("File could not be truncated\n");
	}

	if(ioctl(dev_fd, 0x12345679) == -1)// end receiving data, close the connection
	{
		perror("ioclt client exits error\n");
		return 1;
	}
	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;

	printf("Slave transmission time: %lf ms, File size: %ld \n", trans_time, file_size / 8);

	close(file_fd);
	close(dev_fd);
	return 0;
}
