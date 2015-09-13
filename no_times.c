#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char **argv){
	FILE *fd, *fd2;
	char buffer[4096];

	if (argc<3){
		printf("Usage: ./no_times [1] [2]\n");
		printf("	[1]		input hts transcription file with durations\n");
		printf("	[2]	    output hts transcription file without durations\n");
		exit(0);
	}
	fd = fopen(argv[1],"r");
	fd2 = fopen(argv[2],"w");

	if (!fd){
		printf("%s: no such file",argv[1]);
		exit(0);
	}

	if (!fd2){
		printf("cannot create file %s",argv[1]);
		exit(0);
	}

	long long int a,b;
	fscanf(fd, "%lld %lld ", &a, &b);
	while(fgets(buffer, 4096, fd)){
		fputs(buffer, fd2);
		fscanf(fd, "%lld %lld ", &a, &b);
	}
	
	return 1;
}		
		
		
