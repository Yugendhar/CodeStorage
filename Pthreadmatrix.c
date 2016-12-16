#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>

int rowsperthread;
int n,nt;
int **A;
int **B;
int **P;

void performmultiply(int begin,int size)
{
        int i,j,k,sum;
        for(i=begin;i<size;i++)
        { 
         for(j=0;j<n;j++)
         {
	  sum=0;
          for(k=0;k<n;k++)
          {
                sum+=A[i][k]*B[k][j];
	  }
         P[i][j]=sum;
         }
        }
}

void *matrixMultiply(void *threadid)
{
long tid;
int i,j,k,sum;
int row_begin;
int currentCalrows;
tid = (long)threadid;
printf("Thread created %ld \n",tid);
row_begin=tid*rowsperthread;
currentCalrows=row_begin+rowsperthread;
printf("Calculating Row starting with index:%d \n",row_begin);
int remCalrows;
int lstThread;
int totalCalrows;
if(tid+1<nt)
{
performmultiply(row_begin,currentCalrows);
}
else
{
lstThread=nt-1;
totalCalrows=lstThread*rowsperthread;
remCalrows=row_begin+(n-totalCalrows);
performmultiply(row_begin,remCalrows);
}

for(i=0;i<n;i++)
{
for(j=0;j<n;j++)
{
printf("%d  ",P[i][j]);
}
printf("\n");
}
pthread_exit(NULL);
}

int **allocateMatrix(int n)
{
int **matrix;
int i;
matrix = (int **)malloc((unsigned)n*sizeof(int*));
if(!matrix)
{
//print error
}

for(i=0;i<n;i++)
{
 matrix[i]=(int *)malloc((unsigned)n*sizeof(int));
}
return matrix;
}

void printMatrix(int** matrix)
{

printf("Matrix: \n");
 int i,j;
for(i=0;i<n;i++)
{

for(j=0;j<n;j++)
{
printf("%d  ",matrix[i][j]);
}
printf("\n");
}
}

void AddContents(int** matrix)
{
int i,j;
for(i=0;i<n;i++)
for(j=0;j<n;j++)
{
matrix[i][j]=rand()%10;
}

}
//Main method
void  main()
{

	//seeding random value everytime
//using srand(time(NULL)) or srand(getpid())
srand(time(NULL));//seeding
n=rand()%10;
//Handle zero random  use cases.
n=(n==0)?n=2:n;
nt=rand()%n;
nt=(nt==0)?nt=1:nt;

int i,j,k,sum;
printf("N value for Matrix multiplication : %d \n",n);
printf("Number of threads : %d\n",nt);

A=(int **)allocateMatrix(n);
B=(int **)allocateMatrix(n);
P=(int **)allocateMatrix(n);

AddContents(A);
AddContents(B);

printMatrix(A);
printMatrix(B);
printMatrix(P);
long t;
int rc;

//slicing P into 'nt'(no of threads) of contiguous rows of size 
//which is floor n/nt
//Each thread has certain collection of rows.
//If 6 rows and 3 threads, Each thread has 2 rows to calculate in parallel
//If 8 rows and 3 threads, 2 threads has task to calculate 2 rows and remaining
//1 thread has 4 rows(or remaining).

rowsperthread=n/nt;

pthread_t threads[n];
for(t=0;t<nt;t++)
{
rc=pthread_create(&threads[t],NULL, matrixMultiply ,(void*)t);
}
for(t=0;t<nt;t++)
rc=pthread_join(threads[t],NULL);

printf("\nResult Matrix after Product\n--(P=A*B) Matrix P---: \n");
printMatrix(P);
pthread_exit(NULL);
}




