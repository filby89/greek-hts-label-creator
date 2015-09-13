#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This program creates a htk transcription file for the creation of label transcription hts files.

#define htk_norm 10000000
#define PERIOD_DUR 2.0
#define COMMA_DUR 1.0

typedef struct wut {
	float mean_dur;
	char *name;
	struct wut *next;
}mdur;



int main(int argc, char **argv){

/* Usage: ./main arguments
arguments: 
	argument 1		stressed input text		iso-8859-1 input
	argument 2		input text			iso-8859-1 input
	argument 3		ups2htk phonesetmap		iso-8859-1 input
	argument 4		htk2hts phonesetmap		w/e doesn't matter
	argument 5 		output lab file			optional in the future
	argument 6		phoneme durations file		w/e doesn't matter
*/

	if (argc<7){
		printf("create_lab_file: not enough input arguments.\n");
		printf("Usage: ./main [1] [2] [3] [4] [5]\n");
		printf("	[1]			input stressed file in iso-8859 \n");
		printf("	[2]		    	    input text file in iso-8859 \n");
		printf("	[3]			ups2htk phonesetmap in iso-8859 \n");
		printf("	[4]				   htk2hts phonesetmap  \n");
		printf("	[5]	    output htk transcription file with durations\n");
		printf("	[6]	   	      file containing phoneme durations \n");
		exit(0);
	}
	
	FILE *source, *pmapf, *output, *real, *fdur, *htk2hts, *greek;
	char *str;
	str = (char*)malloc(10*sizeof(char));
	int i,j;

	source = fopen(argv[1],"r");		//Open input stressed file 
	greek = fopen(argv[2],"r");		//Open greek input file
	pmapf = fopen(argv[3],"r"); 		//Open ups2htk phonesetmap
	htk2hts = fopen(argv[4],"r");		//Open htk2hts phonesetma
	real = fopen(argv[5],"w");		//Open output lab file
	fdur = fopen(argv[6],"r");		//Open phoneme durations file

/// create hts2hts phonesetmap

	char ***k2s;
	k2s = (char***)malloc(50*sizeof(char**));
	
	for (i=0;i<50;i++)
		k2s[i] = (char**)malloc(2*sizeof(char*));
	
	for (i=0;i<50;i++)
		for (j=0;j<2;j++)
		k2s[i][j] = (char*)malloc(10*sizeof(char));

	for (i=0;i<30;i++){
		fscanf(htk2hts,"%s", k2s[i][0]);
		fscanf(htk2hts,"%s", k2s[i][1]);
	}


	for (i=0;i<30;i++){
//		printf("%s ", k2s[i][0]);
//		printf("%s \n", k2s[i][1]);
	}

/// create ups2hts phonesetmap

	char ***pmap;
	pmap = (char***)malloc(50*sizeof(char**));
	
	for (i=0;i<50;i++)
		pmap[i] = (char**)malloc(2*sizeof(char*));
	

	for (i=0;i<50;i++)
		for (j=0;j<2;j++)
		pmap[i][j] = (char*)malloc(10*sizeof(char));
	


	for (i=0;i<37;i++){
		fscanf(pmapf,"%s", pmap[i][0]);
			if (strcmp(pmap[i][0],"h")==0){
				pmap[i][1] == "h";
				continue;
			}	
		fscanf(pmapf,"%s", pmap[i][1]);
	}


	for (i=0;i<37;i++){
//		printf("%s ", pmap[i][0]);
//		printf("%s \n", pmap[i][1]);
	}


// Create list containing mean duration of each phoneme

	mdur *head, *tmp;



	tmp = (mdur*)malloc(sizeof(mdur));
	head = tmp;
	
	tmp->name = (char*)malloc(10*sizeof(char));
	
	while (fscanf(fdur,"%s %f", tmp->name, &tmp->mean_dur)==2){
		tmp->next=(mdur*)malloc(sizeof(mdur));
		tmp=tmp->next;
		tmp->name = (char*)malloc(10*sizeof(char));
	}
	
	tmp->next = NULL;

	tmp = head;

	if (!tmp){
		printf("error filling list\n");
		exit(-1);
	}
/*
	while(tmp->next){
		++i;
		printf("%s %f\n", tmp->name, tmp->mean_dur);
		tmp=tmp->next;
	}
*/

//////////////////////////////// create transcription file ///////////////////////////////

	float temp_duration;
	long long int start_time=0;
	long long int end_time=0;
	int flag = 0;
	end_time = 0.5*htk_norm;
	fprintf(real,"%lld %lld %s %s\n", start_time, end_time, "sil", "silence");
	int str_iteration=0;
	char *temp_str;
	temp_str = (char*)malloc(10*sizeof(char));
	char temp_char;

	size_t string_length;

	temp_char=fgetc(source);
	
	sprintf(temp_str,"%c",temp_char);

	if (strcmp(temp_str," ")==0){
		temp_char=fgetc(source);
		sprintf(temp_str,"%c",temp_char);
	}

	char *buf;
	buf = (char*)malloc(20*sizeof(char));
	char *greek_word;
	greek_word = (char*)malloc(20*sizeof(char));	
	int first_time = 1;
	fscanf(greek,"%s",greek_word);	
	long long int temp;
	int period=0;
	int comma=0;

	while (temp_char!=-1){
		if (strcmp(temp_str,"_")==0){
			comma = 1;
		}

		if (strcmp(temp_str,".")==0)
			period=1;
		

		

		if (strcmp(temp_str," ")==0){
			flag = 1;
			fscanf(greek,"%s",greek_word);
//get rid of commas and periods in the end of the word
			string_length = strlen(greek_word);
			if (greek_word[string_length-1] == ',' || greek_word[string_length-1] == '.'){
 			greek_word[string_length-1] = 0;  
			}


//print sp with duration in case of periods and commas

			if (period){
			temp = end_time + htk_norm*PERIOD_DUR; 						
			fprintf(real,"%lld %lld %s\n", end_time, temp, "sp");
			end_time = temp;
			period=0;
			}

			else if (comma){
//uncomment to add mean pau duration for comma
/*				tmp = head;
				while (tmp->next){
					if (strcmp("pau", tmp->name)==0){
						temp = end_time + htk_norm*(tmp->mean_dur); 	
						fprintf(real,"%lld %lld %s\n", end_time, temp, "sp");
						end_time = temp;
					}
					tmp = tmp->next;
				}*/
			temp = end_time + htk_norm*COMMA_DUR; 						
			fprintf(real,"%lld %lld %s\n", end_time, temp, "sp");
			end_time = temp;

			comma=0;
			}
		
			else
				fprintf(real,"%lld %lld %s\n", end_time, end_time, "sp");

			buf[0] = '\0';
			buf = (char*)malloc(20*sizeof(char));
			while (temp_char!=-1 && strcmp(temp_str," ")==0){
				temp_char = fgetc(source);
				sprintf(temp_str,"%c",temp_char);
			}
		}

		for (i=0;i<37;i++)				
			if (strcmp(pmap[i][0],temp_str)==0)
				for (j=0;j<30;j++)
					if (strcmp(pmap[i][1],k2s[j][0])==0){
						tmp = head;
						while(tmp->next){
							if (strcmp(k2s[j][1],tmp->name)==0){
								start_time = end_time;
								end_time = htk_norm*(tmp->mean_dur) + start_time;
								fprintf(real,"%lld %lld %s", start_time, end_time, pmap[i][1]);
								if (flag || first_time){
									fprintf(real," %s\n",greek_word);
									flag = 0;
								}
								else fprintf(real, "\n");
							}
							tmp=tmp->next;
							}

						}
		temp_char=fgetc(source);
		sprintf(temp_str,"%c",temp_char);
		if (first_time)	
			first_time=0;

	}


	fprintf(real,"%lld %lld %s\n", end_time, end_time, "sp");
	start_time=end_time;
	end_time = 0.5*htk_norm + start_time;
	fprintf(real,"%lld %lld %s %s", start_time, end_time, "sil", "silence");

	fcloseall();


	return 1;
}












