#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){

	char file_name[25] = "loremipsum.txt";
	char buffer[100];
	FILE*fp;
	int count = 0;
	double totalTime = 0.0;
	
	int c;
	fp = fopen(file_name, "r");

	clock_t start;
	start = clock();
	while ((c= getc(fp)) != EOF){
		if (c == 'a'){
			count++;
		}
	}
	clock_t stop = clock();
	totalTime = ((double)(stop-start))/CLOCKS_PER_SEC;
	fclose(fp);

	printf("Total time is %f seconds\n", totalTime);


	printf("number of a's is %d", count);



	return 0;

}