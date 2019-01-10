/* structure is used to return two values from minMax() */
#include<stdio.h> 
#include <malloc.h>
typedef struct MinMaxPair 
{ 
	int min; 
	int max; 
}MinMaxPair; 

typedef struct DS{
	int *arr;
	int n;
}DS;

MinMaxPair getMinMax(int arr[], int low, int high) 
{ 
	MinMaxPair minmax, mmleft, mmright;	 
	int mid; 
	
	/* If there is only one element */
	if (low == high){ 
		minmax.max = arr[low]; 
		minmax.min = arr[low];	 
		return minmax; 
	}	 
	
	/* If there are two elements */
	if (high == low + 1){ 
		if (arr[low] > arr[high]){ 
			minmax.max = arr[low]; 
			minmax.min = arr[high]; 
		} 
		else{ 
			minmax.max = arr[high]; 
			minmax.min = arr[low]; 
		} 
		return minmax; 
	} 
	
	/* If there are more than 2 elements */
	mid = (low + high)/2; 
	mmleft = getMinMax(arr, low, mid); 
	mmright = getMinMax(arr, mid+1, high); 

	
	/* compare minimums of two parts*/
	if (mmleft.min > mmright.min){  
		mmleft.min = mmright.min; 
	}
	/* compare maximums of two parts*/
	if (mmleft.max < mmright.max){
	  	mmleft.max = mmright.max; 
	}

	return mmleft; 
} 

DS *getArrayFromDisk(char const *fileName){
	FILE *fp;
	DS *a;
	a = malloc(sizeof(DS));

	fp = fopen(fileName, "r");

	if(fp == NULL){
		printf("Error opening file!\n");
		return NULL;
	}
	if(feof(fp)){
		printf("Premature end of file\n");
		return NULL;
	}

	fscanf( fp,"%d", &a->n);

	a->arr = malloc(sizeof(int) * a->n);
	printf("Array is:\n");
	for(int i = 0; i < a->n;i++ ){
		fscanf(fp, "%d", &a->arr[i]);
		printf("%d\t",a->arr[i] );
		if(feof(fp)){
			printf("Yoda\n");
			a->n = i;
		}
	}
	printf("\n-------------\n");
	fclose(fp);

	return a;

}

/* Driver program to test above function */
int main(int argc, char const *argv[])
{
	FILE *fp;

	DS *d = getArrayFromDisk(argv[1]);
	if(d == NULL){
		printf("Error\n");
		return 0;
	}
	MinMaxPair myMinmax = getMinMax(d->arr, 0, d->n-1); 
	printf("\nMinimum element is %d", myMinmax.min); 
	printf("\nMaximum element is %d", myMinmax.max); 
	getchar();
	return 0;	
} 
