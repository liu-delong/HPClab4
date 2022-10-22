#include<iostream>
#include<pthread.h>
#include<cmath>
#include<algorithm>
using namespace std;
long long int __NAN=0x7FF8000000000000;
#define nan (*(double*)&__NAN)
double a2;
double delta;
double minusb;
double x1,x2;
bool a2_ok=false,delta_ok=false,minusb_ok=false,x_ok=false;
pthread_cond_t cond_a2,cond_delta,cond_minusb,cond_x;
pthread_mutex_t lock_a2,lock_delta,lock_minusb,lock_x;
double a,b,c;
void* cal_a2(void* arg)
{
    a2=2*a;
    pthread_mutex_lock(&lock_a2);
    a2_ok=true;
    pthread_cond_signal(&cond_a2);
    pthread_mutex_unlock(&lock_a2);
    return NULL;
}
void* cal_delta(void* arg)
{
    delta=b*b-4*a*c;
    pthread_mutex_lock(&lock_delta);
    delta_ok=true;
    pthread_cond_signal(&cond_delta);
    pthread_mutex_unlock(&lock_delta);
    return NULL;
}
void* cal_minusb(void* arg)
{   
    minusb=-b;
    pthread_mutex_lock(&lock_minusb);
    minusb_ok=true;
    pthread_cond_signal(&cond_minusb);
    pthread_mutex_unlock(&lock_minusb);
    return NULL;
}
void* cal_x(void* arg)
{
    pthread_mutex_lock(&lock_a2);
    while(!a2_ok)
    {
        pthread_cond_wait(&cond_a2,&lock_a2);
    }
    pthread_mutex_unlock(&lock_a2);

    pthread_mutex_lock(&lock_delta);
    while(!delta_ok)
    {
        pthread_cond_wait(&cond_delta,&lock_delta);
    }
    pthread_mutex_unlock(&lock_delta);

    pthread_mutex_lock(&lock_minusb);
    while(!minusb_ok)
    {
        pthread_cond_wait(&cond_minusb,&lock_minusb);
    }
    pthread_mutex_unlock(&lock_minusb);
    pthread_mutex_lock(&lock_x);
    if(delta<0)
    {
        x1=nan;
        x2=nan;
    }
    else
    {
        x1=(minusb+sqrt(delta))/a2;
        x2=(minusb-sqrt(delta))/a2;
    }
    x_ok=true;
    pthread_cond_signal(&cond_x);
    pthread_mutex_unlock(&lock_x);
    return NULL;
}
int main(int argc,char** argv)
{
    if(argc==4)
    {
        a=stod(argv[1]);
        b=stod(argv[2]);
        c=stod(argv[3]);
    }
    else
    {
        cout<<"please input a b c:";
        cin>>a>>b>>c;
    }
    pthread_t thread[4];
    pthread_mutex_init(&lock_a2,NULL);
    pthread_mutex_init(&lock_delta,NULL);
    pthread_mutex_init(&lock_minusb,NULL);
    pthread_mutex_init(&lock_x,NULL);
    pthread_cond_init(&cond_a2,NULL);
    pthread_cond_init(&cond_delta,NULL);
    pthread_cond_init(&cond_minusb,NULL);
    pthread_cond_init(&cond_x,NULL);
    pthread_create(thread+0,NULL,cal_a2,NULL);
    pthread_create(thread+1,NULL,cal_minusb,NULL);
    pthread_create(thread+2,NULL,cal_delta,NULL);
    pthread_create(thread+3,NULL,cal_x,NULL);
    pthread_mutex_lock(&lock_x);
    while(!x_ok)
    {
        pthread_cond_wait(&cond_x,&lock_x);
    }
    cout<<"x1="<<x1<<" "<<"x2="<<x2<<endl;
    pthread_mutex_unlock(&lock_x);
}
