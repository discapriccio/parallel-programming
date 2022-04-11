#include <iostream>
#include<arm_neon.h>
using namespace std;

const int N=10;
float init[N][N];
float m1[N][N];
float m2[N][N];

void m_reset(int** m)
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

void ord_GE(int** m)  //串行高斯消去
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

void NEON_GE(int** m)  //NEON SIMD 并行高斯消去
{
    for(int k=0;k<N;k++)
    {
        float32x4_t vt,va;
        vt=vdupq_n_f32(m[k][k]);
        int j=0;
        for(j=k+1;j+4<N;j+=4)
        {
            va=vld1q_f32(m[k][j]);
            va=vdivq_f32(va,vt);
            vst1q_f32(m[k][j],va);
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
                vij=vld1q_f32(m[i][j]);
                vkj=vld1q_f32(m[k][j]);
                vik=vmulq_f32(vik,vkj);
                vij=vsubq_f32(vij,vik);
                vst1q_f32(m[i][j],vij);
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

int main()
{
    cout << "Hello world!" << endl;
    m_reset(init);
    m_reset(m1);
    m_reset(m2);
    ord_GE(m1);
    NEON_GE(m2);
    bool flag=check(m1,m2);
    if(flag==1) cout<<"验证通过"<<endl;
    else cout<<"验证失败"<<endl;
    cout<<"NEON并行化高斯消去结果："<<endl;
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++) cout<<m2[i][j]<<"  ";
        cout<<endl;
    }
    return 0;
}
