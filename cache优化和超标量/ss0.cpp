#include<iostream>
#include<sys/time.h>
using namespace std;

const long long N=1<<17;     //取2的n次幂，方便递归设计
int arr[N];
unsigned long long ans;
unsigned long diff;
int cir=1000;

void commonFunc()
{
    ans=0;
    for(int i=0;i<N;i++)
    {
        ans+=arr[i];
    }

}

int main()
{
    for(int i=0;i<N;i++) arr[i]=i;

    struct timeval tv_begin,tv_end;
    gettimeofday(&tv_begin,NULL);
    for(int k=0;k<cir;k++)
    {
        commonFunc();
    }
    gettimeofday(&tv_end,NULL);
    diff = 1000000 * (tv_end.tv_sec-tv_begin.tv_sec)+ tv_end.tv_usec-tv_begin.tv_usec;
    cout<<(diff/1000.0)/cir<<"ms"<<endl;
}
