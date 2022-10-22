#include<pthread.h>
#include<iostream>
#include<cstdio>
#include"js.h"
#include"matrix_tool.h"
#include<algorithm>
#include<cmath>
using namespace std;
struct gemm_arg
{
    int *A,*B,*C;
    int M,N,K;
};

void* gemm(void *_arg)
{
    auto arg=(gemm_arg*)_arg;
    auto A=arg->A;
    auto B=arg->B;
    auto C=arg->C;
    auto M=arg->M;
    auto N=arg->N;
    auto K=arg->K;
    delete arg;
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<K;j++)
        {
            C[i*K+j]=0;
            for(int k=0;k<N;k++)
            {
                C[i*K+j]+=A[i*N+k]*B[k*N+j];
            }
        }
    }
    return NULL;
}
int main(int argc,char** argv)
{
    int M,N,K,T;
    if(argc==5)
    {
        M=stoi(argv[1]);
        N=stoi(argv[2]);
        K=stoi(argv[3]);
        T=stoi(argv[4]);
    }
    else
    {
        printf("please input M N K T:");
        scanf("%d %d %d %d",&M,&N,&K,&T);
    }
    
    int **A=create_matrix(M,N);
    int **B=create_matrix(N,K);
    int **C=create_matrix(M,K);
    init_matrix(M,N,A);
    init_matrix(N,K,B);

    jser timer;
    if(T==1)
    {
        timer.begin();
        auto arg=new gemm_arg;
        *arg={A[0],B[0],C[0],M,N,K};
        gemm(arg);
    }
    else
    {
        timer.begin();
        pthread_t thread[K];
        int avgM=ceil((double)M/T);
        for(int i=0;i<T-1;i++)
        {
            auto arg=new gemm_arg;
            *arg={A[i*avgM],B[0],C[i*avgM],avgM,N,K};
            pthread_create(&thread[i],NULL,gemm,arg);
        }
        auto arg=new gemm_arg;
        *arg={A[(T-1)*avgM],B[0],C[(T-1)*avgM],M-(T-1)*avgM,N,K};
        pthread_create(&thread[T-1],NULL,gemm,arg);
        for(int i=0;i<T;i++)
        {
            pthread_join(thread[i],NULL);
        }
    }
    auto cost=timer.end_ns();
    if(M<=5)
    {
        cout<<"A"<<endl;
        print_matrix(A,M,N);
        cout<<"B"<<endl;
        print_matrix(B,N,K);
        cout<<"C"<<endl;
        print_matrix(C,M,K);
    }
    
    cout<<M<<" "<<T<<" "<<cost<<endl;
}