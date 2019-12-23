#include <iostream>
#include <time.h>
#include <stdlib.h>
void Count_sort(int a[],int n,int thread_count) {
	int i, j, count;
	int* temp = (int*)malloc(n*sizeof(int));
#pragma omp parallel num_threads(thread_count) private(i,j,count) shared(temp)
	for (int i = 0; i < n;i++) {
		count = 0;
		for (int j = 0; j < n;j++) {
			if (a[j]< a[i]) {
				count++;
			}
			else if (a[j] == a[i] && j<i) {
				count++;
			}
		}
		temp[count] = a[i];
	}
	memcpy(a,temp,n*sizeof(int));
	for (int i = 0; i < n; i++)
	{
		printf("%d ",*(a+i));
	}
	printf("\n");
	free(temp);
}


int comp(const void* a, const void* b)
{
	return *(int*)a - *(int*)b;
}

int main(int argc,char*argv[]) {

	clock_t start_time, end_time;
	

	int lenth;
	lenth = 5000;
	printf("length is %d\n",lenth);

	int *a = (int*)malloc(lenth*sizeof(int));
	srand(time(0));
	for (int i = 0; i < lenth ;i++) {
		*(a + i) = rand() / 200;
		printf("%d ", *(a + i));
	}
	printf("\n");
	int thread_count = 5;

	start_time = clock();
	//qsort(a,lenth,sizeof(int),comp);
	Count_sort(a,lenth,thread_count);
	end_time = clock();
	double Times = (double)(end_time - start_time)/CLOCKS_PER_SEC;
	printf("%lf seconds\n", Times);


}