#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct MotEntry{
	int no;
	char mnemonic[10];
}MotEntry;

int getMotEntryFromFile(FILE *file, MotEntry *a){
	if(!feof(file)){
		fscanf(file, "%d\t%s", &a->no, a->mnemonic);
		return 1;
	}
	return 0;
}

int getIndexForEntry(char searchString[]){
	FILE *fp = fopen("mot.txt", "r");
	MotEntry entry;
	if(fp == NULL){
		printf("Error opening file\n");
		return -1;
	}
	while(getMotEntryFromFile(fp, &entry) != 0){
		if(strcmp(entry.mnemonic,searchString) == 0){
			fclose(fp);
			return entry.no;
		}
	}
	fclose(fp);

	return -1;
}

int main(int argc, char const *argv[])
{
	char searchString[10];
	int result;


	printf("Enter an element to search\n");
	scanf("%s", searchString);
	result = getIndexForEntry(searchString);
	if(result == -1){
		printf("Entry not available in MOT table\n");
	}else{
		printf("Entry available at %d\n", result); 
	}

	return 0;
}


