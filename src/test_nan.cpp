#include<iostream>
using namespace std;
long long int __NaN=0x7FF8000000000000;
int main()
{
    double nan=*(double*)&__NaN;
    double nan2=0x7FF8000000000000;
    cout<<*(double*)&__NaN<<endl;
}