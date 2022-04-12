#include <iostream>
#include<arm_neon.h>
#include<sys/time.h>
using namespace std;

const int N=10;
float** init;
float** m1;
float** m2;
unsigned long long ans;
unsigned long diff;
int cir=10000;

void m_reset(float** m)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<i;j++) m[i][j]=0.0;
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
            m[i][k]=0.0;
        }
    }
}

void NEON_GE(float** m)  //NEON SIMD 并行高斯消去
{
    for(int k=0;k<N;k++)
    {
        float32x4_t vt,va;
        vt=vdupq_n_f32(m[k][k]);
        int j=0;
        for(j=k+1;j+4<N;j+=4)
        {
            va=vld1q_f32(m[k]+j);
            va=vdivq_f32(va,vt);
            vst1q_f32(m[k]+j,va);
        }
        for(j;j<N;j++) m[k][j]=m[k][j]/m[k][k];  //处理剩下不足四个的
        m[k][k]=1.0;
        for(int i=k+1;i<N;i++)
        {
            float32x4_t vik,vij,vkj;
            vik=vdupq_n_f32(m[i][k]);
            int j=0;
            for(j=k+1;j+4<N;j+=4)
            {
                vij=vld1q_f32(m[i]+j);
                vkj=vld1q_f32(m[k]+j);
                vik=vmulq_f32(vik,vkj);
                vij=vsubq_f32(vij,vik);
                vst1q_f32(m[i]+j,vij);
            }
            for(j;j<N;j++) m[i][j]=m[i][j]-m[i][k]*m[k][j];  //处理剩下不足四个的
            m[i][k]=0;
        }
    }
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
	
	gettimeofday(&tv_begin,NULL);
	for(int k=0;k<cir;k++)
		ord_GE(m1);
	gettimeofday(&tv_end,NULL);
    diff = 1000000 * (tv_end.tv_sec-tv_begin.tv_sec)+ tv_end.tv_usec-tv_begin.tv_usec;
    cout<<"ordinary:"<<(diff/1000.0)/cir<<"ms"<<endl;
	
	gettimeofday(&tv_begin,NULL);
	for(int k=0;k<cir;k++)
		NEON_GE(m2);
	gettimeofday(&tv_end,NULL);
    diff = 1000000 * (tv_end.tv_sec-tv_begin.tv_sec)+ tv_end.tv_usec-tv_begin.tv_usec;
    cout<<"simd:"<<(diff/1000.0)/cir<<"ms"<<endl;
	
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
