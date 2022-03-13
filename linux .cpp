#include<iostream>
#include<sys/time.h>
#include<string>
using namespace std;
const int N=200;
int res[N];
int mat[N][N];
int arr[N];

void commonFunc()
{
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
    cout<<(tv_begin.tv_usec - tv_end.tv_usec)<<endl;
    cout<<-((tv_begin.tv_usec - tv_end.tv_usec)/1000.0)/cir<<"ms"<<endl;

}

void cacheOptiFunc()
{
    struct timeval tv_begin,tv_end;
    gettimeofday(&tv_begin,NULL);
    int cir=1000;
    for(int k=0;k<cir;k++)
    {
        //for(int i=0;i<N;i++) res[i]=0;
        memset(res,0,sizeof(int)*N);
        for(int j=0;j<N;j++)
            for(int i=0;i<N;i++)
                res[i]+=mat[j][i]*arr[j];
    }
    gettimeofday(&tv_end,NULL);
    cout<<(tv_begin.tv_usec - tv_end.tv_usec)<<endl;
    cout<<-((tv_begin.tv_usec - tv_end.tv_usec)/1000.0)/cir<<"ms"<<endl;
}


int main()
{
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            mat[i][j]=i+j;    // Ϊ������֤��ȷ�ԣ���ÿһ��Ԫ����Ϊi+j
    for(int i=0;i<N;i++) arr[i]=1;

    cout<<"ƽ���㷨��"<<endl;
    commonFunc();
    for(int i=0;i<10;i++) cout<<res[i]<<" ";
    cout<<endl;

    cout<<"�Ż��㷨��"<<endl;
    cacheOptiFunc();
    for(int i=0;i<10;i++) cout<<res[i]<<" ";

}
