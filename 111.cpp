#include<iostream>
#include<windows.h>
using namespace std;
const int N=10;
int* res;
int** mat;
int* arr;

void commonFunc()
{
    for(int j=0;j<N;j++)
    {
        res[j]=0;
        for(int i=0;i<N;i++)
            res[j]+=mat[i][j]*arr[i];
    }
}

void cacheOptiFunc()
{
    for(int i=0;i<N;i++) res[i]=0;
    for(int j=0;j<N;j++)
        for(int i=0;i<N;i++)
            res[i]+=mat[j][i]*arr[j];
}


int main()
{
    mat=new int* [N];
    for(int i=0;i<N;i++) mat[i]=new int[N];
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            mat[i][j]=i+j;    // Ϊ������֤��ȷ�ԣ���ÿһ��Ԫ����Ϊi+j
    arr=new int[N];
    res=new int[N];
    for(int i=0;i<N;i++) arr[i]=1;
    commonFunc();
    cout<<"ƽ���㷨��"<<endl;
    for(int i=0;i<N;i++) cout<<res[i]<<" ";
    cout<<endl;
    cacheOptiFunc();
    cout<<"�Ż��㷨��"<<endl;
    for(int i=0;i<N;i++) cout<<res[i]<<" ";

}
