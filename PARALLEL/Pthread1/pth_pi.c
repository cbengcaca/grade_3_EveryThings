/* File:     pth_pi.c
 * Purpose:  Try to estimate pi using the formula
 *
 *              pi = 4*[1 - 1/3 + 1/5 - 1/7 + 1/9 - . . . ]
 *
 *           This version has a *very serious bug*
 *
 * Compile:  gcc -g -Wall -o pth_pi pth_pi.c -lm -lpthread
 * Run:      ./pth_pi <number of threads> <n>
 *           n is the number of terms of the series to use.  
 *           n should be evenly divisible by the number of threads
 * Input:    none
 * Output:   Estimate of pi as computed by multiple threads, estimate
 *           as computed by one thread, and 4*arctan(1).
 *
 * Notes:
 *    1.  The radius of convergence for the series is only 1.  So the 
 *        series converges quite slowly.
 *
 * IPP:   Section 4.4 (pp. 162 and ff.)
 */        

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#pragma comment(lib, "pthreadVC2.lib")

#define PI acos(-1)


//线程数量
long thread_count;
//划分块数
long long blockNum;
double sum;

//梯形高度
double height;
double start;
double end;

void* Thread_sum(void* rank);
int Thread_getAB(double num);

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);

double* arraySquare;

int main(int argc, char* argv[]) {
    long thread;
    double* arraySquareIn = malloc(blockNum*sizeof(double));
    arraySquare = arraySquareIn;

    pthread_t* thread_handles;

    /* Get number of threads from command line */
    Get_args(argc, argv);

   
   //申请线程空间
   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   //SUM
   sum = 0.0;


   for (thread = 0; thread < thread_count; thread++) {
       pthread_create(&thread_handles[thread], NULL,
           Thread_sum, (void*)thread);
   }
        
   for (thread = 0; thread < thread_count; thread++) {
       pthread_join(thread_handles[thread], NULL);
   }
       
   for (int i = 0; i < blockNum;i++) {
       sum += *(arraySquare + sizeof(double)*i);
   }

   printf("total square is %lf\n", sum);
   
   free(thread_handles);
   
   return 0;
}  /* main */

/*根据输入获取梯形边长*/
int Thread_getAB(double num) {
    double indexA = start + num* height;
    double indexB = start + (num+1) * height;
    double heightA = sin(indexA)+5;
    double heightB = sin(indexB) + 5;
    double x = (heightA + heightB) / 2;
    return x;
}

/*------------------------------------------------------------------
 * Function:       Thread_sum
 * Purpose:        Add in the terms computed by the thread running this 
 * In arg:         rank
 * Ret val:        ignored
 * Globals in:     n, thread_count
 * Global in/out:  sum 
 */
void* Thread_sum(void* rank) {
    double sum;

    for (int i = rank; i < blockNum;) {
        printf("read block %d\n",i);
        sum = Thread_getAB(i)*height;
        *(arraySquare + sizeof(double)*i) = sum;
        i += thread_count;
    }
   return NULL;
}  /* Thread_sum */



/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) {
    if (argc != 5) exit(1);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0) exit(1);
   blockNum = strtoll(argv[2], NULL, 10);
   if (blockNum <= 0) exit(1);
   start = strtol(argv[3],NULL,10);
   end = strtol(argv[4],NULL,10);
   if (start > end) {
       printf("start is greater than end!\n");
       exit(1);
   }
   printf("input access complite\n");
   height = (end - start) / blockNum;

}  /* Get_args */

