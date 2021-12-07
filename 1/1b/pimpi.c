#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define SEED 3345342

int main(int argc, char **argv){
    int iter = 1000000,i, count = 0, reduced_count,rank, reduced_iter;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    double x,y,pi,z,t1,t2;
    
    if(rank==0) t1 = MPI_Wtime();
    if(rank>0){
        srand(SEED);
        for(i=0;i<iter;i++){
            x = (double) rand()/RAND_MAX;
            y = (double) rand()/RAND_MAX;
            z = x*x + y*y;
            if(z<=1) count++;
        }
        
    }
    MPI_Reduce(&count, &reduced_count,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        // MPI_Reduce(&iter, &reduced_iter,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        // reduced_iter -= iter;
    if(rank==0){
        t2 = MPI_Wtime();
        pi = (double) reduced_count/(double)iter*4;
        printf("PI: %g in time %lf\n",pi,t2-t1);
    }
    MPI_Finalize();
    return 0;
    
}


