#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define FALSE 0
#define TRUE 1

int counter = 0;

char queen[] = {0xE2, 0x99, 0x95};

void printNQueens(int *array, int n){
	int i, j;
	printf("\n");
	int k = 0;
	for(i=0;i<n;i++){
		k = 0;
		for(j=0;j<n;j++){
			if((k + i) % 2 == 0){
				printf("\e[48;5;%dm", 242 );
			}
			k++;
			if(array[i] == j){
				printf("\e[38;2;%d;%d;%dm",255,0,0  );
				printf("| %c%c%c |", queen[0], queen[1], queen[2]);
				// write(1, queen, 3);
				// printf("|");
				printf("\e[38;2;%d;%d;%dm",0,255,0  );
			}else{
				printf("|--|");
			}

			// printf("|  |", (array[i] == j) ? 'Q': '-' );
			printf("\e[0m");
		}
		printf("\n");
	}
	printf("\n");
}

int canPlace(int *array, int k, int i){
	int j;

	for(j=0; j < k; j++){
		if(array[j] == i ||
			abs(j-k) == abs(array[j] - i))
		{
			return FALSE;
		}
	}
	return TRUE;
}

void nQueens(int *array, int k, int n){
	int i = 0;
	for(i=0;i<n;i++){
		if(canPlace(array, k, i) == TRUE){	
			array[k] = i;
			if(k == n-1){
				counter += 1;
				printf("Solution: %d\n",counter );
				printNQueens(array, n);
			}else{
				nQueens(array, k+1, n);
			}
		}
	}
}



int main(int argc, char const *argv[])
{
	int array[MAX];
	int i;
	int choice;
	int n;
	printf("Enter number of queens:\n");
	scanf("%d", &n);
	if(n < 0 || n == 2 || n == 3){
		printf("Invalid Configuration\n");
		return 0;
	}

	printf("Do you want to enter initial config? (1 or 0):\n");
	scanf("%d", &choice);
	if(choice == 1){
		for(i = 0; i < n; i++){
			printf("Enter Column Number of Queen (-1 for no queen) %d\n",i+1);
			scanf("%d", &array[i]);
			if(array[i] == -1){
				nQueens(array, i-1, n);
				break;
			}
		}
	}else{
		nQueens(array, 0, n);
	}

	

	

	return 0;
}