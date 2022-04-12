#include<iostream>
#include <nmmintrin.h> //SSSE4.2
#include<windows.h>
using namespace std;

const int N=10;
float** init;
float** m1;
float** m2;
unsigned long long head, tail, freq;  //timers
int cir=1000;

void m_reset(float** m)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++) m[i][j]=0;
        m[i][i]=1.0;
        for(int j=i+1;j<N;j++) m[i][j]=rand();
    }
    for(int k=0;k<N;k++)
    {
        for(int i=k+1;i<N;i++)
        {
            for(int j=0;j<N;j++) m[i][j]+=m[k][j];
        }
    }
}

void ord_GE(float** m)  //串行高斯消去
{
    for(int k=0;k<N;k++)
    {
        for(int j=k+1;j<N;j++) m[k][j]=m[k][j]/m[k][k];
        m[k][k]=1.0;
        for(int i=k+1;i<N;i++)
        {
            for(int j=k+1;j<N;j++) m[i][j]=m[i][j]-m[i][k]*m[k][j];
            m[i][k]=0;
        }
    }
}
void SSE_GE_unaligned(float** m)  //SSE SIMD 不对齐
{
    for(int k=0;k<N;k++)
    {
        __m128 vt,va;
        vt=_mm_set1_ps(m[k][k]);
        int j=0;
        for(j=k+1;j+4<N;j+=4)
        {
            va=_mm_loadu_ps(m[k]+j);
            va=_mm_div_ps(va,vt);
            _mm_storeu_ps(m[k]+j,va);
        }
        for(j;j<N;j++) m[k][j]=m[k][j]/m[k][k];  //处理剩下不足四个的
        m[k][k]=1.0;
        for(int i=k+1;i<N;i++)
        {
            __m128 vik,vij,vkj;
            vik=_mm_set1_ps(m[i][k]);
            int j=0;
            for(j=k+1;j+4<N;j+=4)
            {
                vij=_mm_loadu_ps(m[i]+j);
                vkj=_mm_loadu_ps(m[k]+j);
                vik=_mm_mul_ps(vik,vkj);
                vij=_mm_sub_ps(vij,vik);
                _mm_storeu_ps(m[i]+j,vij);
            }
            for(j;j<N;j++) m[i][j]=m[i][j]-m[i][k]*m[k][j];  //处理剩下不足四个的
            m[i][k]=0;
        }
    }
}

bool check(int** m1,int** m2)
{
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if(abs(m1[i][j]-m2[i][j])>10e-6)
                return false;
    return true;
}
bool check(float** m1,float** m2)
{
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if(abs(m1[i][j]-m2[i][j])>10e-6)
                return false;
    return true;
}

int main()
{
    struct timeval tv_begin,tv_end;

	init=new float*[N];
	for(int i=0;i<N;i++) init[i]=new float[N];
	m1=new float*[N];
	for(int i=0;i<N;i++) m1[i]=new float[N];
	m2=new float*[N];
	for(int i=0;i<N;i++) m2[i]=new float[N];

    m_reset(init);
    m1=init;
	m2=init;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
	for(int k=0;k<cir;k++)
		ord_GE(m1);
	QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<"ordinary:"<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;

	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
	for(int k=0;k<cir;k++)
		SSE_GE_unaligned(m2);
	QueryPerformanceCounter((LARGE_INTEGER*)&tail );
    cout<<"SSE unaligned:"<<( tail-head) * 1000.0 / freq /cir <<"ms"<<endl;

    bool flag=check(m1,m2);
    if(flag==1) cout<<"correct"<<endl;
    else cout<<"wrong"<<endl;
    //cout<<"ordinary result："<<endl;
    //for(int i=0;i<N;i++)
    //{
    //    for(int j=0;j<N;j++) cout<<m1[i][j]<<"  ";
    //    cout<<endl;
    //}

	//cout<<"simd result："<<endl;
    //for(int i=0;i<N;i++)
    //{
    //    for(int j=0;j<N;j++) cout<<m2[i][j]<<"  ";
    //    cout<<endl;
    //}
    return 0;
}
