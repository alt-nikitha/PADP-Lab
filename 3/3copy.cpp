#include<math.h>
#include<iostream>
#include<omp.h>
using namespace std;
long Striker(bool *composite, long i, long stride, long limit){
    for(;i<=limit;i+=stride){
        composite[i] = true;
    }
    return i;
}

long unfriendly(long n){
    long m = (long)sqrt((double)n);
    long count = 0;
    bool *composite = new bool[n+1];
    double t = omp_get_wtime();
    for(long i=2;i<=m;i++){
        if(!composite[i]){
            count++;
            Striker(composite,2*i,i,n);
        }
    }
    for(long i=m+1;i<=n;i++){
        if(!composite[i]) count++;
    }
    printf("time unfriendly  : %lf\n", omp_get_wtime()-t);
    return count;
}
long friendly(long n){
    long m = (long)sqrt((double)n);
    long *factor = new long[m];
    long *striker = new long[m];
    bool *composite = new bool[n+1];
    memset(composite,0,n);
    long count = 0;
    long n_factor = 0;
    double t = omp_get_wtime();
    for(long i=2;i<=m;i++){
        if(!composite[i]){
            ++count;
            striker[n_factor] = Striker(composite, 2*i, i,m);
            factor[n_factor++] = i;
        }
    }
    for(long window=m+1;window<=n;window+=m){
        long limit = min(window+m-1,n);
        for(long k = 0; k<n_factor;k++){
            striker[k] = Striker(composite,striker[k], factor[k], limit);
        }
        for(long i = window; i<=limit;i++){
            if(!composite[i]) count++;
        }
    }
    printf("time friendly : %lf\n", omp_get_wtime()-t);
    return count;

}

long friendly_parallel(long n){
    long m = (long)sqrt((double)n);
    long *factor = new long[m];
    long *striker = new long[m];
    bool *composite = new bool[n+1];
    memset(composite,0,n);
    long count = 0;
    long n_factor = 0;
    double t = omp_get_wtime();
    #pragma omp single
    {
        for(long i=2;i<=m;i++){
        if(!composite[i]){
            ++count;
            striker[n_factor] = Striker(composite, 2*i, i,m);
            factor[n_factor++] = i;
        }
    }
    }
    
    #pragma omp parallel for reduction(+:count) shared(composite, striker, factor)
    for(long window=m+1;window<=n;window+=m){
        long limit = min(window+m-1,n);
        for(long k = 0; k<n_factor;k++){
            striker[k] = Striker(composite,striker[k], factor[k], limit);
        }
        for(long i = window; i<=limit;i++){
            if(!composite[i]) count++;
        }
    }

    printf("time friendly parallel : %lf\n", omp_get_wtime()-t);

    return count;

}

long parallel_sieve(long n){
    long m = (long)sqrt((double)n);
    long count =0;
    long n_factor = 0;
    long *factor = new long[n];
    double t = omp_get_wtime();
    #pragma omp parallel
    {
        bool *composite = new bool[m+1];
        long *striker = new long[m];
        #pragma omp single
        {
            memset(composite,0,m);
            for(long i=2;i<=m;i++){
                if(!composite[i]){
                    ++count;
                    Striker(composite,2*i,i,m);
                    factor[n_factor++] = i;
                }
            }
        }
        long base = -1;
        #pragma omp for reduction(+:count)
            for (long window=m+1;window<=n;window+=m){
                memset(composite,0,m);
                if(base!=window){
                    base = window;
                    for(long k=0;k<n_factor;++k){
                        striker[k] = (base+factor[k]-1)/factor[k]*factor[k] - base;
                    }
                    
                }
                long limit = min(window+m-1,n) - base;
                for(long k=0;k<n_factor;k++){
                    striker[k] = Striker(composite, striker[k], factor[k],limit) -m;
                }
                for(long i=0;i<=limit;i++){
                    if(!composite[i]) ++count;
                }
                base+=m;
            }
    }
    printf("time parallel : %lf\n", omp_get_wtime()-t);
    return count;
}
int main(int argc, char **argv){
    int n;
    cout<<"Enter the limit"<<endl;
    cin>> n;
    int count = parallel_sieve(n);
    cout<<count<<endl;
    count = friendly(n);
    cout<<count<<endl;
    count = unfriendly(n);
    cout<<count<<endl;
    count = friendly_parallel(n);
    cout<<count<<endl;


    return 0;
}