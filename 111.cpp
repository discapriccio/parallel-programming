#include<iostream>
using namespace std;
const int R=10;  //����
const int C=10;  //����
void commonFunc()
{

}
}
int main()
{
    int* mat=new int[R];
    for(int i=0;i<R;i++) mat[i]=new int[C];
    for(int i=0;i<R;i++)
        for(int j=0;j<C;j++)
            mat[i][j]=1;    // Ϊ������֤��ȷ�ԣ���ÿһ��Ԫ����Ϊ1
    int arr=new int[R];
    for(int i=0;i<R;i++) arr[i]=1;
}
