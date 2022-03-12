#include<iostream>
#include<windows.h>
using namespace std;
const int N=2000;
int res[N];
int mat[N][N];
int arr[N];
long long head, tail, freq;  //timers

void commonFunc()
{

    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    int cir=1000;
    for(int k=0;k<cir;k++)
    {
        for(int j=0;j<N;j++)
        {
            res[j]=0;
            for(int i=0;i<N;i++)
                res[j]+=mat[i][j]*arr[i];
        }
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;

}

void cacheOptiFunc()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    int cir=1000;
    for(int k=0;k<cir;k++)
    {
        //for(int i=0;i<N;i++) res[i]=0;
        memset(res,0,sizeof(int)*N);
        for(int j=0;j<N;j++)
            for(int i=0;i<N;i++)
                res[i]+=mat[j][i]*arr[j];
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;
}


int main()
{
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            mat[i][j]=i+j;    // 为方便验证正确性，将每一个元素设为i+j
    for(int i=0;i<N;i++) arr[i]=1;

    cout<<"平凡算法："<<endl;
    commonFunc();
    for(int i=0;i<10;i++) cout<<res[i]<<" ";
    cout<<endl;

    cout<<"优化算法："<<endl;
    cacheOptiFunc();
    for(int i=0;i<10;i++) cout<<res[i]<<" ";

}
