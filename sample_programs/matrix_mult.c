#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) 
{
int   i, n,j,m,k;
m = 4;
n = 2;
float a[n][m], b[m][n], c[n][n]; 



printf("Matrix 1\n");
for (i=0;i<n;i++){
    for (j=0;j<m;j++){
        a[i][j]= i*j*1.0;
        printf("%f ",a[i][j]);

    }
    printf("\n");
}

printf("Matrix 2\n");
for (i=0;i<m;i++){
    for (j=0;j<n;j++){
        b[i][j]= i*j*1.0;
        printf("%f ",b[i][j]);
    }
    printf("\n");
}

#pragma omp parallel for private(i,j,k) shared(a,b,c)
    for(i=0;i<n;i++)    
    {    
        for(j=0;j<n;j++)    
        {    
            c[i][j]=0;    
            for(k=0;k<m;k++)    
            {    
                c[i][j]+=a[i][k]*b[k][j];    
            }    
        }    
    }    




printf("Product:\n");
for (i=0;i<n;i++){
    for (j=0;j<n;j++){
        printf("%f ",c[i][j]);
    }
    printf("\n");
}


}
