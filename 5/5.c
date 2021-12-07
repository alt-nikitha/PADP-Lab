#include<stdio.h>

#include<stdlib.h>
#define BUF_SIZE 32
#include<string.h>
#include<mpi.h>

int main(int argc, char**argv){
    char *m[5] = {"\nHello","\nRVCE","\nCSE","\n7th Sem","\nHPC"};
    char mess[BUF_SIZE];
    int root =0, tag=0;
    int numProcs, myRank, i;
    MPI_Init(&argc,&argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    if(myRank==0){
        for(i=1;i<numProcs;i++){
            strcpy(mess,m[i-1]);
            MPI_Send(&mess,BUF_SIZE, MPI_CHAR,i,tag,MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(&mess, BUF_SIZE, MPI_CHAR, root, tag, MPI_COMM_WORLD, &status);
        printf("I am process %d , Recvd message: %s from %d\n", myRank, mess, root);
    }
    MPI_Finalize();
    return 0;
}