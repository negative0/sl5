#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define TRUE 1
#define FALSE 0
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

void readNextLine(char *readLine, FILE *fp){
	// printf("Ftel %d\n",ftell(fp) );
	fgets(readLine, 100, fp);
	// return readLine;
}

FILE *openFile(const char *fileName){
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL){
		printf("Error opening file\n");
	}
	return fp;
}

int isValidCombo(char a, char b){
	switch(a){
		case '+':
			if(b == '+')
				return TRUE;
		case '-':
			if(b == '-')
				return TRUE;
		case '/':
		case '*':
		case '<':
			if(b=='<')
				return TRUE;

		case '>':
			if(b=='>')
				return TRUE;
		case '=':
			if(b == '=')
				return TRUE;
				break;
	
	}

	return FALSE;

}

int isDelimiter(char a){
	switch (a){
		case ' ':
		case '=':
		case '+':
		case '-':
		case '%':
		case '/':
		case '<':
		case '>':
		case '{':
		case '}':
		case '[':
		case ']':
		case '(':
		case ')':
		case '*':
		case '!':
		case '~':
		case '|':
		case '\'':
		case '"':
		case ':':
		case ';':
		case '\n':
			return TRUE;
			break;
		default:
			return FALSE;
	}
}



int main(int argc, char const *argv[])
{
	char readLine[100];
	char buffer[10];
	int buf_ptr;
	FILE* fp;
	int i,x;

	int left=0, right=0, left2 = 0;


	if((fp = openFile(argv[1])) != NULL){

		while(!feof(fp)){

			readNextLine(readLine, fp);
			// printf("Line:%s\n",readLine );
			int len = strlen(readLine);
			left = 0, right = 0;
			for(i = 0; i < strlen(readLine); i++){
				// printf("%c\n", readLine[i]);

				if(isDelimiter(readLine[i]) == TRUE){
					// printf("d:%c\n",readLine[i] );
					if( i != len && isDelimiter(readLine[i+1]) == TRUE){

						if(isValidCombo(readLine[i], readLine[i+1])){
							// Two character operators
							printf("dd:%c%c\t", readLine[i], readLine[i+1]);
							// printf("Buffer:\n");
							buf_ptr = 0;
							for(x=left; i< right;x++){
								// printf("x%c", readLine[x]);
								buffer[buf_ptr++] = readLine[x];
							}
							// buffer[buf_ptr] = '\0';

							printf("Buffer:%s\n", buffer);
							i++;
							left = i+1;
							right = left;
						}else{
							//  Not a double char
							printf("d:%c\t", readLine[i]);
							buf_ptr = 0;
							for(x=left; i< right;x++){
								// printf("x%c", readLine[x]);
								buffer[buf_ptr++] = readLine[x];
							}
							// buffer[buf_ptr] = '\0';

							printf("Buffer:%s\n", buffer);


							left = i+1;
							right = left;

									
						}
					}else{
						buf_ptr = 0;
						for(x=left; i< right;x++){
							// printf("x%c", readLine[x]);
							buffer[buf_ptr++] = readLine[x];
						}
						buffer[buf_ptr] = '\0';

						printf("Buffer:%s\n", buffer);
						left = i+1;
						right = left;
					}
				}else{
					//Not a delimiter
					printf("nd:%c\t",readLine[i]);
					right++;
				}
			}

		}

	}else{
		printf("Error in opening file\n");
	}

	return 0;
}