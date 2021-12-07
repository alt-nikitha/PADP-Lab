#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
    int n,i,j,k;
    for(n=500;n<=2000;n+=500){
        int *arr1[n],*arr2[n],*arr3[n];
        for(i=0;i<n;i++){
            arr1[i] = (int*)malloc(n*sizeof(int));
            arr2[i] = (int*)malloc(n*sizeof(int));
            arr3[i] = (int*)malloc(n*sizeof(int));

        }
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                arr1[i][j] = rand()%10;
                arr2[i][j] = rand()%10;
            }
        }
        int l;
        for(l=1;l<=8;l=l<<1){
            double t1,t2;
            t1 = omp_get_wtime();
            omp_set_num_threads(l);
            #pragma omp parallel for private(j,k)
            for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                    arr3[i][j] = 0;
                    for(k=0;k<n;k++){
                        arr3[i][j]+=arr1[i][k]*arr2[k][j];
                    }
                }
            }
            t2 = omp_get_wtime();
            printf("Time taken for %d threads, %lf\n",l,t2-t1);
        }

    }
    return 0;
}