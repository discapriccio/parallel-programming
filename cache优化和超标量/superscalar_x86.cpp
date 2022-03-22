#include<iostream>
#include<windows.h>
using namespace std;

const long long N=1<<17;     //取2的n次幂，方便递归设计
int arr[N];
unsigned long long ans;
unsigned long long head, tail, freq;  //timers
int cir=1000;

void commonFunc()
{
    ans=0;
    for(int i=0;i<N;i++)
    {
        ans+=arr[i];
    }

}
void ssOpti1_2()
{
    ans=0;
    unsigned long long sum1=0,sum2=0;
    for(int i=0;i<N;i+=2)
    {
        sum1+=arr[i];
        sum2+=arr[i+1];
    }
    ans=sum1+sum2;
}

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


void ssOpti2()  //super scalar optimize
{
    unsigned long long temp[N];
    for(int i=0;i<N;i++) temp[i]=arr[i];
    for(int m=N;m>1;m/=2)
    {
        for(int i=0;i<m/2;i++)
        {
            temp[i]=temp[i*2]+temp[i*2+1];
        }
    }
    ans=temp[0];
}

int main()
{
    for(int i=0;i<N;i++) arr[i]=i;

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for(int k=0;k<cir;k++)
    {
        commonFunc();
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;
    cout<<ans<<endl;


    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for(int k=0;k<cir;k++)
    {
        ssOpti1_2();
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;
    cout<<ans<<endl;

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for(int k=0;k<cir;k++)
    {
        ssOpti1_3();
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;
    cout<<ans<<endl;


    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    for(int k=0;k<cir;k++)
    {
        ssOpti2();
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;
    cout<<ans<<endl;
}
