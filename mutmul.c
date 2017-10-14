#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 512

int main(int argc,char*argv[]){

	int *A,*B,*Ar;
	float* C,*D;
	int myrank,P,i,j,k;
	double tstart,tend;
	MPI_Status status;

	MPI_Init (&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
	MPI_Comm_size(MPI_COMM_WORLD,&P);

	int perProcessor = SIZE/P;
	
	
	B = (int*) malloc(SIZE*SIZE*sizeof(int));
	Ar  = (int*)malloc(sizeof(int)*SIZE*perProcessor);
	D = (float*) malloc(sizeof(float)*SIZE*perProcessor);
	if(myrank==0){

		C = (float*) malloc(SIZE*SIZE*sizeof(float));
		A = (int*) malloc(SIZE*SIZE*sizeof(int));
		
		int n = 0;
		int i,j;
		for(i=0;i<SIZE;i++){
			for(j=0;j<SIZE;j++){
				int index = i*SIZE + j; 	
				A[index] = n++;
			}			
		}

		for(i=0;i<SIZE;i++){
			for(j=0;j<SIZE;j++){
				int index = i*SIZE + j;
				B[index]= n++;
			}
		}

		//print_matrix(A);
		//printf("\n************************\n");
		//print_matrix(B);
		//printf("\n************************\n");	
		tstart = MPI_Wtime();
	}		 

	MPI_Bcast(B,SIZE*SIZE,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Scatter(A,SIZE*SIZE/P,MPI_INT,Ar,SIZE*SIZE/P,MPI_INT,0,MPI_COMM_WORLD);
	
	for(i=0;i<perProcessor;i++)
		for(j=0;j<SIZE;j++){
			D[i*SIZE+j] = 0.0f;
			for(k=0;k<SIZE;k++){
			

				D[i*SIZE+j] += Ar[i*SIZE+k]*B[k*SIZE+j];
			}
		}		 

	MPI_Gather(D, SIZE*SIZE/P, MPI_FLOAT,C,SIZE*SIZE/P,MPI_INT,0,MPI_COMM_WORLD);
	

	if(myrank==0){
		tend = MPI_Wtime();
		printf("finished in %lf seconds\n",tend-tstart);
		//print_matrix(C);
	}

	MPI_Finalize();
	return 0;

}
