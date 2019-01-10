#include<stdio.h>


int mergeCompare(int *a, int n){
	int *min = malloc(sizeof(int) * n/2);
	int *max = malloc(sizeof(int) * n/2);
	int s_size = 2;
	for(int i = 0; i < n; i+=2){
		if(a[i] > a[i+1]){
			min[i/2] = a[i];
			max[i/2] = a[i+1];
		}else{
			min[i/2] = a[i+1];
			max[i/2] = a[i];
		}
	}
	for(int x = n/2; x > 1; x /= 2){
		if(min[x] < min[x+1]){
			min[x] = min[x+1];
		}
		if(max[x] > max[x+1]){
			max[x] = max[x+1];
		}
	}
}

int main(int argc, char const *argv[])
{
	
	return 0;
}