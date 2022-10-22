#include<random>
#include<cstdio>
#include<iostream>
#include<ctime>
#include<pthread.h>
using namespace std;
int N=100000;
int T=4;
int a=0;
uniform_real_distribution<double> u;
default_random_engine e;
pthread_mutex_t lock_a;
void* func(void* arg)
{
    int times=N/T;
    for(int i=0;i<times;i++)
    {
        double x=u(e);
        double y=u(e);
        if(y<x*x)
        {
            pthread_mutex_lock(&lock_a);
            a++;
            pthread_mutex_unlock(&lock_a);
        }
    }
    return NULL;
}
int main()
{
    u=uniform_real_distribution<double>(0,1);
    e=default_random_engine(time(0));
    pthread_mutex_init(&lock_a,NULL);
    pthread_t thread[T];
    for(int i=0;i<T;i++)
    {
        pthread_create(thread+i,NULL,func,NULL);
    }
    for(int i=0;i<T;i++)
    {
        pthread_join(thread[i],NULL);
    }
    cout<<(double)a/N<<endl;
}