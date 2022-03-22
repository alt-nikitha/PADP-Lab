#include <math.h>
#include <cstring>
#include <omp.h>
#include <iostream>
using namespace std;
#define NUM_THREADS 2

long striker(bool *composite, long start, long stride, long end){
    for(;start<=end; start+=stride){
        composite[start] = true;
    }
    return start;
}


long cacheunfriendly(long n){
    long count = 0;
    long m = (long)sqrt((double)n);
    bool *composite = new bool[n+1];
    double t = omp_get_wtime();
    memset(composite,0,n);
    for(long i = 2;i<=m;i++){
        if(!composite[i]){
            count++;
            striker(composite,2*i, i, n);
        }
    }


    for(int i = m+1; i<=n;i++){
        if(!composite[i]){
            count++;
        }
    }
    printf("time taken: %f\n", omp_get_wtime()-t);
    return count;
}

long cachefriendly(long n){
    long count = 0;
    long m = (long)sqrt((double)n);
    bool *composite = new bool[n+1];
    long *factor = new long[m+1];
    long *strike = new long[m+1];
    long n_factor = 0;
    double t = omp_get_wtime();
    memset(composite,0,n);
    for(long i = 2;i<=m;i++){
        if(!composite[i]){
            count++;
            strike[n_factor] = striker(composite,2*i, i, m);
            factor[n_factor++] = i;
        }
    }

    for(long window = m+1; window<=n; window+=m){
        long limit = min(window+m-1,n);
        for(int k = 0; k<n_factor;k++){
            strike[k] = striker(composite, strike[k], factor[k], limit);
        }

        for(int i = window; i<=limit;i++){
            if(!composite[i]) count++;
        }
    }
    printf("time taken: %f\n", omp_get_wtime()-t);
    return count;
}

long parallel_sieve(long n){
    long count = 0;
    long m = (long)sqrt((double)n);
    long *factor = new long[n];
    long n_factor = 0;
    double t = omp_get_wtime();
    omp_set_num_threads(2);
    #pragma omp parallel
    {
        bool *composite = new bool[m+1];
        long *strike = new long[m];

        #pragma omp single
        {
            memset(composite,0,m);
            for(long i = 2;i<=m;i++){
                if(!composite[i]){
                    ++count;
                    striker(composite,2*i, i, m);
                    factor[n_factor++] = i;
                }
            }
        }

        long base = -1;
        #pragma omp for reduction(+:count)
        for(long window=m+1;window<=n;window+=m){
            memset(composite,0,m);
            if(base!=window){
                base = window;
                for(long k = 0;k<n_factor;k++){
                    strike[k] = (base+factor[k]-1)/factor[k]*factor[k]-base;
                }
            }
           
            long limit = min(window+m-1, n) - base;
            for(long k = 0;k<n_factor;k++){
                strike[k] = striker(composite,strike[k], factor[k], limit) - m;
            }

            for(int i = 0; i<=limit;i++){
                if(!composite[i]) ++count;
            }

            



            base+=m;
        }

        


    }
    printf("time taken: %f\n", omp_get_wtime()-t);
    return count;

    

}





int main(){

    long count;
    for(long n=10; n<=1000000; n*=10){
        cout <<"Limit :"<<n<<"\n";
        count = cacheunfriendly(n);
        cout<< "Num primes: "<<count<<"\n";
        

        count = cachefriendly(n);
        cout<< "Num primes: "<<count<<"\n";
        

        count = parallel_sieve(n);
        cout<< "Num primes: "<<count<<"\n";
        
    }

    return 0;

}