#include<stdio.h>
#include<string.h>
#include <malloc.h>
#define MOT_TABLE "mot.txt"
#define LIT_TAB "lit.txt"
#define SYM_TAB "sym.txt"

/*
	format for Intermediate Code:
	LC class opcode op1_type op1_code op2_type op2_code 
*/

typedef struct Statement{
	char label[10], mnemonic[10], op1[10], op2[10];
}Statement;

typedef struct LitEntry{
	int no, address;
	char literal[3];
}LitEntry;

typedef struct SymEntry{
	int no, size, address;
	char symbol[10];
}SymEntry;

typedef struct PoolEntry{
	int no, pool;
}PoolEntry;

typedef struct DS {
	FILE *input;
	LitEntry litTab[10];
	SymEntry symTab[10];
	PoolEntry poolTab[5];

	int litPtr, symPtr, poolPtr, lc;
}DS;

typedef struct MotEntry{
	char mnemonic[10], class[3];
	int code, length;
}MotEntry;

MotEntry *getMotForMnemonic(char search[]){
	FILE *mot;
	mot = fopen("mot.txt", "r");
	MotEntry *entry = malloc(sizeof(MotEntry));
	while(1){
		fscanf(mot, "%s\t%d\t%s\t%d", entry->mnemonic, &entry->code, entry->class, &entry->length);
		if(feof(mot)){ return NULL; }
		if(strcmp(search, entry->mnemonic) == 0){
			return entry;
		}
	}
	return NULL;
}

DS *initNewDataStructure(const char *filename){
	DS *ds = malloc(sizeof(DS));
	ds->litPtr = 0;
	ds->symPtr = 0;
	ds->poolPtr = 1;
	ds->lc = 0;
	ds->input = fopen(filename, "r");
	ds->poolTab[0].no = 1;
	ds->poolTab[0].pool = 1;
	return ds;
}

Statement *getNextStatement(DS *ds){
	Statement *statement = malloc(sizeof(Statement));
	fscanf(ds->input, "%s\t%s\t%s\t%s", statement->label, statement->mnemonic, statement->op1, statement->op2);
	if(feof(ds->input)){ statement = NULL; return NULL; }
	return statement;
}

void dumpToFile(){
	//Dump all the tables to disk
}

SymEntry *getSymTabEntry(DS *ds, char label[]){
	int i =0;
	for(i = 0; i < ds->symPtr; i++){
		if(strcmp(ds->symTab[i].symbol, label) == 0){
			return &(ds->symTab[i]);
		}
	}
	return NULL;
}
LitEntry *getlitTabEntry(DS *ds, char label[]){
	int i =0;
	for(i = 0; i < ds->litPtr; i++){
		if(strcmp(ds->litTab[i].literal, label) == 0){
			return &(ds->litTab[i]);
		}
	}
	return NULL;
}


int main(int argc, char const *argv[])
{	

	DS *myDS = NULL;
	Statement *statement = NULL;
	MotEntry *motEntry = NULL;
	SymEntry *symEntry = NULL;
	LitEntry *litEntry = NULL;

	if(argc != 2){
		printf("Specify filename of asm\n");
	}

	myDS = initNewDataStructure(argv[1]);
	int isStarted = 0;
	while((statement = getNextStatement(myDS)) != NULL){
		motEntry = getMotForMnemonic(statement->mnemonic);
		// Condition for START
		if(strcmp(statement->mnemonic, "START")){
			printf("\nStart of Program\n");
			isStarted = 1;
		}

		if(strcmp(statement->label, "--") != 0){
			//Label Exists
			symEntry = getSymTabEntry(myDS, statement->label);
			if(symEntry == NULL){
				// Does not exist. Add to SymTab
				/*int no, size, address;
					char symbol[10];*/
				myDS->symTab[myDS->symPtr].no = myDS->symPtr;
				myDS->symTab[myDS->symPtr].size = 0;
				myDS->symTab[myDS->symPtr].address = -1;
				strcpy(myDS->symTab[myDS->symPtr].symbol, statement->label);
				myDS->symPtr++;

			}else{
				// Entry Exists in symtab
			}
		}

		// Check for Imperative statement
		if(strcmp(motEntry->class,"IS") == 0){
			myDS->lc = myDS->lc + motEntry->length;
			switch(statement->op1[0]){
				case '=':
				// op1 is literal, handle error
				printf("Op1 is literal\n");
				return 0;
				break;
			}

			switch(statement->op2[0]){
				case '=':
				// op1 is literal, add literal
				if((litEntry = getlitTabEntry(myDS, statement->op2)) == NULL){
					// Entry Does not exist
					/*int no, address;
						char literal[3];*/
					int cur = myDS->symPtr;
					myDS->litTab[cur].no = myDS->litPtr;
					myDS->litTab[cur].address = myDS->lc;
					strcpy(myDS->litTab[cur].literal, statement->op2);
					myDS->litPtr++;
				}
				break;
			}

		}
		// Condition for END
		if(strcmp(statement->mnemonic, "END")){
			printf("\nEnd of program\n");
			break;
		}


	}
	return 0;
}
