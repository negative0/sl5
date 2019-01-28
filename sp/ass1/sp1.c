#include<stdio.h>
#include<string.h>
#include <malloc.h>
#define MOT_TABLE "mot.txt"
#define LIT_TAB "lit.txt"
#define SYM_TAB "sym.txt"
#define FIRST_OP 0
#define SECOND_OP 1

/*
	format for Intermediate Code:
	LC class opcode op1_type op1_code op2_type op2_code 
*/

typedef struct IntCode
{
	int lc, op1_code, op2_code ;
	char opcode[4], class[4], op1_type[1], op2_type[1];
}IntCode;

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
	if(mot == NULL) { printf("mot.txt file does not exist\n"); return NULL;}
	MotEntry *entry = malloc(sizeof(MotEntry));
	while(1){
		fscanf(mot, "%s\t%d\t%s\t%d", entry->mnemonic, &entry->code, entry->class, &entry->length);
		if(feof(mot)){ return NULL; }
		if(strcmp(search, entry->mnemonic) == 0){
			return entry;
		}
	}
	fclose(mot);
	return NULL;
}

DS *initNewDataStructure(const char *filename){
	// printf("In initNewDataStructure\n");
	DS *ds = malloc(sizeof(DS));
	ds->litPtr = 0;
	ds->symPtr = 0;
	ds->poolPtr = 0;
	ds->lc = 0;
	ds->input = fopen(filename, "r");
	ds->poolTab[0].no = 1;
	ds->poolTab[0].pool = 0;
	// printf("Out initNewDataStructure\n");

	return ds;
}

Statement *getNextStatement(DS *ds){
	// printf("In getNextStatement\n");

	Statement *statement = malloc(sizeof(Statement));
	fscanf(ds->input, "%s\t%s\t%s\t%s", statement->label, statement->mnemonic, statement->op1, statement->op2);
	if(feof(ds->input)){ statement = NULL; return NULL; }
	// printf("Out getNextStatement\n");

	return statement;
}

void dumpToFile(DS *ds){
	//Dump all the tables to disk
	// litTab, symTab, poolTab
	int i = 0;
	FILE *fp;
	fp = fopen(SYM_TAB, "w");
	for(i=0;i<ds->symPtr;i++){
		fprintf(fp, "%d %d %d %s\n", ds->symTab[i].no, ds->symTab[i].size, ds->symTab[i].address, ds->symTab[i].symbol);
	}
	fclose(fp);

	fp = fopen(LIT_TAB, "w");
	for(i=0;i<ds->litPtr;i++){
		fprintf(fp, "%d %s %d\n",ds->litTab[i].no, ds->litTab[i].literal, ds->litTab[i].address );
	}
	fclose(fp);
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

int getRegister(char *abc){
	if(strcmp(abc, "AREG") == 0){
		return 1;
	} else if(strcmp(abc, "BREG") == 0){
		return 2;	
	} else if(strcmp(abc, "CREG") == 0){
		return 3;
	}else if(strcmp(abc, "DREG") == 0){
		return 4;
	}
	return -1;

}

void processLiteralsInPool(DS *myDS, FILE *out){
	int i = myDS->poolTab[myDS->poolPtr].pool;
	LitEntry *litEntry = NULL;
			
	for(; i < myDS->litPtr; i++){
		litEntry = &myDS->litTab[i];
		litEntry->address = myDS->lc;
		myDS->lc += 1;
		fprintf(out, "%d\t-\t-\t%s\n",myDS->lc, litEntry->literal);
	}
}

int main(int argc, char const *argv[])
{	

	DS *myDS = NULL;
	Statement *statement = NULL;
	MotEntry *motEntry = NULL;
	SymEntry *symEntry = NULL, *symEntry1 = NULL;
	LitEntry *litEntry = NULL;
	PoolEntry *poolEntry = NULL;
	int regCode;
	if(argc != 2){
		printf("Specify filename of asm\n");
	}

	myDS = initNewDataStructure(argv[1]);
	FILE *out = fopen("output.txt", "w");
	if(out == NULL){
		printf("Error opening output file\n");
		return 0;
	}
	int isStarted = 0;
	while((statement = getNextStatement(myDS)) != NULL){

		motEntry = getMotForMnemonic(statement->mnemonic);
		// Condition for START
		if(strcmp(statement->mnemonic, "START") == 0){
			printf("\nStart of Program\n");
			isStarted = 1;
			if(strcmp(statement->op1, "-") != 0){
				myDS->lc = atoi(statement->op1);
			}
			fprintf(out, "%d\tAD\t01\tC\t%d\n",myDS->lc, myDS->lc );
			continue;
		}

		// Condition for END
		if(strcmp(statement->mnemonic, "END") == 0){
			printf("\nEnd of program\n");
			//processLiteralsInPool(myDS, out);
			//Print LC
			// fprintf(out, "%d\t",myDS->lc);
			// Print class and opcode
			fprintf(out, "%d\t%s\t%d ", myDS->lc, motEntry->class, motEntry->code);
			dumpToFile(myDS);
			break;
		}

		//Check for label
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
				symEntry->address = myDS->lc;
				// symEntry->size = atoi(statement->op1);
			}
		}

		// Check for LTORG
		if(strcmp(statement->mnemonic, "LTORG") == 0){
			processLiteralsInPool(myDS, out);
		}

		// Check for DECLARATION
		if(strcmp(statement->mnemonic, "DS") == 0 || strcmp(statement->mnemonic, "DC") == 0){

			symEntry = getSymTabEntry(myDS, statement->label);
			if(symEntry == NULL){
				// Does not exist. Add to SymTab
				/*int no, size, address;
					char symbol[10];*/
				myDS->symTab[myDS->symPtr].no = myDS->symPtr;

				if(strcmp(statement->mnemonic, "DS") == 0){
					myDS->symTab[myDS->symPtr].size = atoi(statement->op1);
					myDS->lc+=atoi(statement->op1);
		
				}else{
					myDS->symTab[myDS->symPtr].size = 1;
					myDS->lc+=1;
					
				}

				myDS->symTab[myDS->symPtr].address = myDS->lc;
				strcpy(myDS->symTab[myDS->symPtr].symbol, statement->label);
				myDS->symPtr++;

			}else{
				// Entry Exists in symtab
				symEntry->address = myDS->lc;
				if(strcmp(statement->mnemonic, "DS") == 0){
					// myDS->symTab[myDS->symPtr].size = atoi(statement->op1);
					symEntry->size = atoi(statement->op1);
					myDS->lc += atoi(statement->op1);

				}else{
					symEntry->size = 1;
					myDS->lc+=1;
				}
			}
			fprintf(out, "%d\tDL\t%d\tC\t%s\n",myDS->lc, motEntry->code, statement->op1);
			continue;
		}


		//Print LC
		fprintf(out, "%d\t",myDS->lc);
		// Print class and opcode
		fprintf(out, "%s\t%d ", motEntry->class, motEntry->code);

		//Change LC wrt ORIGIN
		if(strcmp(statement->mnemonic, "ORGIN") == 0){
			myDS->lc = atoi(statement->op1);
			continue;
		}

		// Handle EQU Statement
		if(strcmp(statement->mnemonic, "EQU") == 0){
			symEntry = getSymTabEntry(myDS, statement->label);
			symEntry1 = getSymTabEntry(myDS, statement->op1);
			if(symEntry != NULL && symEntry1 != NULL){
				symEntry1->address = symEntry->address;
			}
			// fprintf(out, "%d\tDL\t%d\tC %s\n",myDS->lc, motEntry->code, statement->op1);
			fprintf(out, "\n");
			continue;
		}
		


		// Check for Imperative statement
		if(strcmp(motEntry->class,"IS") == 0){
			myDS->lc = myDS->lc + motEntry->length;

			//OP1
			switch(statement->op1[0]){
				case '-': break;
				case '=':
				// op1 is literal, handle error
					printf("\nError: Op1 is literal\n");
					return 0;
				break;
				
				case 'A': case 'B': case 'C': case 'D':
				
				regCode = getRegister(statement->op2);
				if(regCode != -1){
					//Valid Register
					fprintf(out, "\tR %d", regCode);
					break;
				}

				default:
					//Check for symbol
				symEntry = getSymTabEntry(myDS, statement->op2);
				if(symEntry == NULL){
					// Does not exist. Add to SymTab
					/*int no, size, address;
						char symbol[10];*/
					myDS->symTab[myDS->symPtr].no = myDS->symPtr;
					myDS->symTab[myDS->symPtr].size = 0;
					myDS->symTab[myDS->symPtr].address = -1;
					strcpy(myDS->symTab[myDS->symPtr].symbol, statement->label);
					symEntry = &myDS->symTab[myDS->symPtr];
					myDS->symPtr++;


				}else{
					// Entry Exists in symtab
					printf("Symbol Exists in SymTab\n");

				}
				fprintf(out, "\tS %d", symEntry->no);
			}

			//OP2
			switch(statement->op2[0]){
				case '-':
				fprintf(out, "\n");
				 break;
				case '=':
				// op1 is literal, add literal
				if((litEntry = getlitTabEntry(myDS, statement->op2)) == NULL){
					// Entry Does not exist
					/*int no, address;
						char literal[3];*/
					int cur = myDS->litPtr;
					myDS->litTab[cur].no = myDS->litPtr;
					myDS->litTab[cur].address = -1;
					strcpy(myDS->litTab[cur].literal, statement->op2);
					litEntry = &myDS->litTab[cur];
					myDS->litPtr++;
					
				}
				fprintf(out, "\tL %d\n", litEntry->no);
				break;

				case 'A': case 'B': case 'C': case 'D':
				
				regCode = getRegister(statement->op2);
				if(regCode != -1){
					//Valid Register
					fprintf(out, "\tR %d\n", regCode);
					break;
				}

				default:
					//Check for symbol
				symEntry = getSymTabEntry(myDS, statement->op2);
				if(symEntry == NULL){
					// Does not exist. Add to SymTab
					/*int no, size, address;
						char symbol[10];*/
					myDS->symTab[myDS->symPtr].no = myDS->symPtr;
					myDS->symTab[myDS->symPtr].size = 0;
					myDS->symTab[myDS->symPtr].address = -1;
					strcpy(myDS->symTab[myDS->symPtr].symbol, statement->label);
					symEntry = &myDS->symTab[myDS->symPtr];
					myDS->symPtr++;


				}else{
					// Entry Exists in symtab
					printf("Symbol Exists in SymTab\n");

				}
				fprintf(out, "\tS %d\n",symEntry->no);
			}
		}

		
	}
	fprintf(out, "\n");
	fclose(out);
	return 0;
}
