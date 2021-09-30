#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main() {
    int count = 0;
	struct stat s;
	int size;
    int fd = open("loremipsum.txt", O_RDONLY);
	double totalTime = 0.0;
	int status = fstat(fd,&s);
	size=s.st_size;
    char *region = (char*)mmap(
        0, size,
        PROT_READ, MAP_FILE|MAP_PRIVATE,
        fd, 0
        );
     clock_t start = clock();
	 int i ;
    for (i = 0; i < size; i++) {
		char c;
		c = region[i];
         if(c =='a'){
             count++;
         }
    }
	clock_t stop = clock();
	totalTime = (stop-start)/CLOCKS_PER_SEC;
	printf("Total time is %f seconds\n", totalTime);

	   printf("number is %d", count);

    
    int unmap_result = munmap(region, size);
    close(fd);
    return 0;
}
