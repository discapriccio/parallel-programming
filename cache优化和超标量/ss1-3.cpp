#include<iostream>
#include<sys/time.h>
using namespace std;

const long long N=1<<17;     //取2的n次幂，方便递归设计
int arr[N];
unsigned long long ans;
unsigned long diff;
int cir=1000;

void ssOpti1_3()
{
    ans=0;
    unsigned long long sum1=0,sum2=0,sum3=0;
    for(int i=0;i<N;i+=3)
    {
        sum1+=arr[i];
        sum2+=arr[i+1];
        sum3+=arr[i+2];
    }
    ans=sum1+sum2+sum3  ;
}

int main()
{
    for(int i=0;i<N;i++) arr[i]=i;

    struct timeval tv_begin,tv_end;
    gettimeofday(&tv_begin,NULL);
    for(int k=0;k<cir;k++)
    {
        ssOpti1_3();
    }
    gettimeofday(&tv_end,NULL);
    diff = 1000000 * (tv_end.tv_sec-tv_begin.tv_sec)+ tv_end.tv_usec-tv_begin.tv_usec;
    cout<<(diff/1000.0)/cir<<"ms"<<endl;
}
