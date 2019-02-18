#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
typedef struct $1
{
	int no;
	char *value;
}LiteralEntry;

typedef struct $2
{
	int no;
	char *value;
}IdEntry;

typedef struct $3
{
	int no;
	char *symbol;
	char class[4];
	int index;
}UstEntry;

// int readNextLine(char *readLine, FILE *fp){
// 	return fscanf(fp, "%[^(\n| |(|{))]", readLine);
// 	// return readLine;
// }

FILE *openFile(const char *fileName){
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL){
		printf("Error opening file\n");
	}
	return fp;
}



int main(int argc, char const *argv[])
{
	char readLine[100];
	FILE *fp = openFile(argv[1]);
	char *last_token;
	const char *delimiters = " \t{}();";
 	// int chars = 1;
 	int token = 1;
 	char *dup = NULL;
 	while(fgets(readLine, 100, fp) != NULL){
 		// readNextLine(readLine, fp);
		// printf("%s\n",readLine);	
		dup = strdup(readLine);
		last_token = strtok( readLine, delimiters );
		char delim_used = dup[readLine - last_token];
 		// char delim_used;
		// printf("Dup:%s\n",du[] );
 		do{
 			// printf("%d:%s ",token++,last_token);
 			printf("%d:%s ",token++,last_token);
 			last_token = strtok(NULL, delimiters);
 			delim_used = dup[readLine - last_token];

 		}while(last_token != NULL);
 		// free(dup);
 		printf("\n");
 	}
	return 0;
}