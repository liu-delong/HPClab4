#include <pthread.h>
#include <cstdio>
#include <iostream>
#include "js.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;
#define N 1000
int a[N];
int aindex=0;
long long int sum=0;
pthread_mutex_t index_l,sum_l;
int n;int amount;
void* sum_func(void * arg)
{
    while(1)
    {
        pthread_mutex_lock(&index_l);
        int my_index=aindex;
        aindex+=amount;
        pthread_mutex_unlock(&index_l);
        if(my_index>=N) break;
        long long int my_sum=0;
        int my_end_index=my_index+amount;
        if(my_end_index>=N) 
        {
            my_end_index=N;
        }
        for(int i=my_index;i<my_end_index;i++)
        {
            my_sum+=a[i];
        }
        pthread_mutex_lock(&sum_l);
        sum+=my_sum;
        pthread_mutex_unlock(&sum_l);
    }
    return NULL;
    
}
int main(int argc,char **argv)
{
    
    if(argc==3)
    {
        n=stoi(argv[1]);
        amount=stoi(argv[2]);
    }
    else
    {
        cout<<"please input thread number n and the amount of number once can get"<<endl;
        cout<<"n amount:";
        cin>>n>>amount;
    }
    srand(time(0));
    for(int i=0;i<N;i++)
    {
        a[i]=rand();
    }
    pthread_mutex_init(&index_l,NULL);
    pthread_mutex_init(&sum_l,NULL);
    sum=0;
    jser timer;
    timer.begin();
    pthread_t thread[n];
    for(int i=0;i<n;i++)
    {
        pthread_create(&thread[i],NULL,sum_func,NULL);
    }
    for(int i=0;i<n;i++)
    {
        pthread_join(thread[i],NULL);
    }
    auto cost=timer.end_ns();
    cout<<sum<<" "<<cost<<endl;
}