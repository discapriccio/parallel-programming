/**
    为简化程序，暂且不考虑矩阵过大无法放入内存的情况。
    为SIMD并行方便，矩阵采用稠密矩阵方式存储
*/
#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
using namespace std;

struct Hang
{
    bool isNull = 0;
    bool isZi = 0;
    unsigned int* hang;
};


vector<Hang> vecHang; //被消元行
vector<unsigned int*> vecZi; //消元子

int N = 64;  //矩阵列数
int segment = ceil(N / 32.0);
int tail = 32 * segment - N;

int cntHang;
void intToBit(unsigned int x,int* bit_x) //整数按位转换成32bit数组
{
    for (int i = 0; i < 32; i++)
    {
        if (x % 2 == 1) bit_x[31 - i] = 1;
        else bit_x[31 - i] = 0;
        x /= 2;
    }
}

unsigned int bitToInt(int* x,int tail)
{
    unsigned int ret=0;
    unsigned int a = 2 << 31;
    for (int i = 0; i < 32 - tail; i++)
    {
        if (x[i] == 1) ret += a;
        a /= 2;
    }
    return ret;
}

bool canGE(unsigned int* x, unsigned int* y)  //可以高斯消去
{
    for (int i = 0; i < segment; i++)
    {
        int* bit_x=new int[32];
        intToBit(x[i], bit_x);
        int* bit_y=new int[32];
        intToBit(y[i], bit_y);
        for (int j = 0; j < 32; j++)
        {
            if (bit_x[j]==1 != bit_y[j]) return false;
            if (bit_x[j] == 1 && bit_y[j] == 1) return true;
        }
    }
    return true;
}

bool isZiFunc(unsigned int* x) //判断被消元行x是否可被升级为消元子
{
    for (int i = 0; i < vecZi.size(); i++)
    {
        for (int j = 0; j < segment; j++)
        {
            int* bit_x=new int[32];
            intToBit(x[j], bit_x);
            int* bit_y = new int[32];
            intToBit(vecZi[i][j], bit_y);
            for (int k = 0; k < 32; k++)
            {
                if (bit_x[k] == 0 && bit_y[k] == 1 || bit_x[k] == 1 && bit_y[k] == 1)
                    return false;
                if (bit_x[k] == 1 && bit_y[k] == 0)
                    return true;
            }
        }
    }
}

void GE(int id, unsigned int* y)  //vecHang[id]为被消元行，y作为消元子消去
{
    //消去
    unsigned int* x = vecHang[id].hang;
    for (int i = 0; i < segment; i++)
    {
        int* bit_x = new int[32];
        intToBit(x[i],bit_x);
        int* bit_y = new int[32];
        intToBit(y[i], bit_y);
        for (int j = 0; j < 32; j++)
        {
            bit_x[i] = (bit_x[i] == bit_y[i] ? 0:1);
        }
    }
    if (isZiFunc(vecHang[id].hang)) //若当前被消元行升级为消元子
    {
        vecHang[id].isZi = 1;
        for (int k = 0; k < cntHang; k++) //此消元子和所有被消元行做消去（简化版）
        {
            if (vecHang[k].isNull == 1 || vecHang[id].isZi == 1) continue;
            GE(k,vecHang[id].hang);
        }
        vecZi.push_back(vecHang[id].hang);
    }
}

int main()
{
    setlocale(LC_ALL, " Chinese-simplified "); // 设置中文环境
    ifstream hangFile("C:\\Users\\xingyu\\Desktop\\data\\Groebner\\测试样例1 矩阵列数130，非零消元子22，被消元行8\\被消元行.txt");
    ifstream ziFile("C:\\Users\\xingyu\\Desktop\\data\\Groebner\\测试样例1 矩阵列数130，非零消元子22，被消元行8\\消元子.txt");
    ifstream resultFile("C:\\Users\\xingyu\\Desktop\\data\\Groebner\\测试样例1 矩阵列数130，非零消元子22，被消元行8\\消元结果.txt");
    if (!hangFile.is_open()) cout << "未成功打开被消元行文件" << endl;
    if (!ziFile.is_open()) cout << "未成功打开消元子文件" << endl;
    if (!resultFile.is_open()) cout << "未成功打开消元结果文件" << endl;

    string temp;
    //读入好麻烦一会再写
//    while(!hangFile.eof())
//    {
//        hangFile>>temp;
//        int iter=0;
//
//    }
    cout << segment << "    " << tail << endl;

    unsigned int* t = new unsigned int[segment];

    //测试用
    Hang tempHang;
    t[0] = 1; t[1] = 0;
    tempHang.hang = t;
    vecHang.push_back(tempHang);
    t[0] = 2; t[1] = 0;
    tempHang.hang = t;
    vecHang.push_back(tempHang);

    t[0] = 1; t[1] = 0;
    tempHang.hang = t;
    vecZi.push_back(t);



    cntHang = 2;
    for (int i = 0; i < cntHang; i++)
    {
        for (int j = 0; j < vecZi.size(); j++)
        {
            if (canGE(vecHang[i].hang, vecZi[j]))
                GE(i, vecZi[j]);
        }
    }

    //输出被消元行结果
    for (int i = 0; i < cntHang; i++)
    {
        if (vecHang[i].isNull) cout << endl;
        else
        {
            for (int j = 0; j < segment; j++)
            {
                int* bit_x = new int[32];
                intToBit(vecHang[i].hang[j], bit_x);
                for (int k = 0; k < 32; k++) cout << bit_x[k];
                cout << " ";
            }
            cout << endl;
        }
    }
}
