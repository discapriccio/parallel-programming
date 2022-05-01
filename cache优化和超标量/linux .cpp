#include<iostream>
#include<sys/time.h>
#include<string>
using namespace std;
const int N=500;
int res[N];
int mat[N][N];
int arr[N];

void commonFunc()
{
    unsigned long diff;
    struct timeval tv_begin,tv_end;
    gettimeofday(&tv_begin,NULL);
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
    gettimeofday(&tv_end,NULL);
    diff = 1000000 * (tv_end.tv_sec-tv_begin.tv_sec)+ tv_end.tv_usec-tv_begin.tv_usec;
    cout<<(diff/1000.0)/cir<<"ms"<<endl;

}

void cacheOptiFunc()
{
    unsigned long diff;
    struct timeval tv_begin,tv_end;
    gettimeofday(&tv_begin,NULL);
    int cir=1000;
    for(int k=0;k<cir;k++)
    {
        for(int i=0;i<N;i++) res[i]=0;
        //memset(res,0,sizeof(int)*N);
        for(int j=0;j<N;j++)
            for(int i=0;i<N;i++)
                res[i]+=mat[j][i]*arr[j];
    }
    gettimeofday(&tv_end,NULL);
    diff = 1000000 * (tv_end.tv_sec-tv_begin.tv_sec)+ tv_end.tv_usec-tv_begin.tv_usec;
    cout<<(diff/1000.0)/cir<<"ms"<<endl;

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
