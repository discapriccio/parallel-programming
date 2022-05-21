//
// Created by xingyu on 2022/4/30.
//

#include "../head/Placement.h"
#include <string.h>
#include<vector>
#include<pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

using namespace std;
void bfsInitPlacement(Net& netParam,vector<vector<int>>& rowCol)  //bfs初步布局
{
    //vector<vector<int>> rowCol;  //记录每行每列的元件id（外层vector为col，内层vector为row）
    bool* isSelected=new bool[netParam.devices.size()];  //元件是否已被安排行列号
    int cnt=0;  //已有多少元件已被安排行列号
    //memset(isSelected,0,sizeof(isSelected));
    memset(isSelected,0,sizeof(bool)*netParam.devices.size());
    vector<int> colVec;
    int curRow=0;
    for(int i=0;i<netParam.devices.size();i++) //遍历找到入度为零的（isStart==true）放到第一列
    {
        if(netParam.devices[i].d.empty()||netParam.devices[i].d.size()==1&&netParam.devices[i].d[0]==i)  //过滤不与其他元件有联系的元件
        {
            cnt++;
            continue;
        }
        if(netParam.devices[i].isStart==1)
        {
            colVec.push_back(i);
            isSelected[i]=1;
            netParam.devices[i].col=0;
            netParam.devices[i].row=curRow++;
            cnt++;
        }
    }
    //cout<<"chuanxing in degree:"<<cnt<<endl;
    rowCol.push_back(colVec);
    int curCol=0;  //目前在从第几列的元件找下一列的元件
    while(1)
    {
        vector<int> colVec;
        //cout<<curCol<<" "<<rowCol[curCol].size()<<endl;
        for(int i=0;i<rowCol[curCol].size();i++)  //遍历当前列的元件
        {
            int id=rowCol[curCol][i];
            int curRow=0;
            for(int j=0;j<netParam.devices[id].d.size();j++)
            {
                int id2=netParam.devices[id].d[j];
                if(isSelected[id2]) continue;
                cnt++;
                colVec.push_back(id2);
                isSelected[id2]= true;
                netParam.devices[id2].col=curCol+1;
                netParam.devices[id2].row=curRow++;
            }
        }
        rowCol.push_back(colVec);
        curCol++;
        //cout<<cnt<<endl;
        //cout<<netParam.devices.size()<<endl;
        if(cnt==netParam.devices.size()) break;
    }

/*    //验证
    cout<<endl;
    cout<<"test"<<endl;
    for(int i=0;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
            cout<<rowCol[i][j]<<" ";
        cout<<"/"<<endl;
    }*/
}

Net net;
bool* isSelected;
int cnt;
vector<int> colVec;
vector<vector<int>> rowCol; //记录每行每列的元件id（外层vector为col，内层vector为row）

int thread_count=8;  //线程数量
typedef struct{
    int	threadId;
} threadParm_t1;

typedef struct{
    int	threadId;
    int size;  //当前列元件数
    int curCol;
} threadParm_t2;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* threadFunc1(void* parm) //线程函数1--遍历找到入度为零的（isStart==true）放到第一列
{
/*    LARGE_INTEGER tm1,tm2,tmc;
    QueryPerformanceFrequency(&tmc);
    QueryPerformanceCounter(&tm1);*/

    threadParm_t1 *p = (threadParm_t1 *) parm;
    //cout<<"hello from"<<p->threadId<<endl;
    int curCnt=0;
    vector<int> curColVec;
    for(int i=p->threadId;i<net.devices.size();i+=thread_count)  //此线程的任务
    {
        //int curRow=0;
        if(net.devices[i].d.empty()||net.devices[i].d.size()==1&&net.devices[i].d[0]==i)  //过滤不与其他元件有联系的元件
        {
            curCnt++;
            continue;
        }
        if(net.devices[i].isStart==1)
        {
            isSelected[i]=1;
            net.devices[i].col=0;
            //net.devices[i].row=curRow++;  不为元件划定行号，不然要加锁时间代价太大
            curColVec.push_back(i);
            curCnt++;
            //cout<<cnt<<endl;
        }
    }
    pthread_mutex_lock(&lock);  //加锁
    cnt+=curCnt;
    colVec.insert(colVec.end(),curColVec.begin(),curColVec.end());
    pthread_mutex_unlock(&lock);  //解锁
    //cout<<"bye from"<<p->threadId<<endl;
/*    QueryPerformanceCounter(&tm2);
    double time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
    cout<<"Time for threadFunc1,id="<<p->threadId<<"  "<<time*1000<<"ms"<<endl<<endl;*/
    pthread_exit(NULL);
}
void* threadFunc2(void* parm) //线程函数2--探查当前列中特定元件的应放到下一列的元件
{
/*    LARGE_INTEGER tm1,tm2,tmc;
    QueryPerformanceFrequency(&tmc);
    QueryPerformanceCounter(&tm1);*/
    threadParm_t2 *p = (threadParm_t2 *) parm;
    //cout<<"hello2 from"<<p->threadId<<endl;
    int curCnt=0;
    vector<int> curColVec;
    for(int i=p->threadId;i<p->size;i+=thread_count)  //此线程的任务
    {
        int id=rowCol[p->curCol][i];
        //int curRow=0;
        for(int j=0;j<net.devices[id].d.size();j++)
        {
            int id2=net.devices[id].d[j];
            if(isSelected[id2]) continue;
            curCnt++;
            curColVec.push_back(id2);
            isSelected[id2]= true;
            net.devices[id2].col=p->curCol+1;
            //net.devices[id2].row=curRow++;  不为元件划定行号，不然要加锁时间代价太大
        }
    }
    pthread_mutex_lock(&lock);  //加锁
    cnt+=curCnt;
    colVec.insert(colVec.end(),curColVec.begin(),curColVec.end());
    pthread_mutex_unlock(&lock);  //解锁
    //cout<<"bye2 from"<<p->threadId<<endl;
/*    QueryPerformanceCounter(&tm2);
    double time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
    cout<<"Time for threadFunc2,id="<<p->threadId<<"  "<<time*1000<<"ms"<<endl<<endl;*/
    pthread_exit(NULL);
}

void bfsInitPlacementParr(Net netParam)  //并行bfs初步布局
{
    //vector<vector<int>> rowCol;  //记录每行每列的元件id（外层vector为col，内层vector为row）
    net = netParam;
    isSelected = new bool[net.devices.size()];  //元件是否已被安排行列号
    cnt = 0;  //已有多少元件已被安排行列号
    memset(isSelected,0,sizeof(bool)*netParam.devices.size());
       
    //vector<int> colVec;

    //pthread_t* thread_handles; //线程句柄
    //thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
    pthread_t thread_handles[thread_count];  //线程句柄
    threadParm_t1 threadParm[thread_count];  //线程参数
    for (int threadId = 0; threadId < thread_count; threadId++) {
        threadParm[threadId].threadId = threadId;
        pthread_create(&thread_handles[threadId], NULL, threadFunc1, (void *) &threadParm[threadId]);
    }
    for (int i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL); //等待工作线程结束
    }
    //cout<<cnt<<endl;
    //cout << "phrase 1 completed" << endl;
    rowCol.push_back(colVec);

    int curCol = 0;  //目前在从第几列的元件找下一列的元件
    while (1) {
        colVec.clear();
        if(rowCol[curCol].size()<50)  //数据规模较小时不并行
        {
            for(int i=0;i<rowCol[curCol].size();i++)  //遍历当前列的元件
            {
                int id=rowCol[curCol][i];
                //int curRow=0;
                for(int j=0;j<netParam.devices[id].d.size();j++)
                {
                    int id2=netParam.devices[id].d[j];
                    if(isSelected[id2]) continue;
                    cnt++;
                    colVec.push_back(id2);
                    isSelected[id2]= true;
                    netParam.devices[id2].col=curCol+1;
                    //netParam.devices[id2].row=curRow++;
                }
            }
        }
        else
        {
            //pthread_t *thread_handles2; //线程句柄
            //thread_handles2 = (pthread_t *) malloc(thread_count * sizeof(pthread_t));
            pthread_t thread_handles2[thread_count];  //线程句柄
            threadParm_t2 threadParm2[thread_count];
            for (int threadId = 0; threadId < thread_count; threadId++) {
                threadParm2[threadId].threadId = threadId;
                threadParm2[threadId].size = rowCol[curCol].size();
                threadParm2[threadId].curCol = curCol;
                pthread_create(&thread_handles2[threadId], NULL, threadFunc2, (void *) &threadParm2[threadId]);
            }
            for (int i = 0; i < thread_count; i++) {
                pthread_join(thread_handles2[i], NULL); //等待工作线程结束
            }
        }
        rowCol.push_back(colVec);
        curCol++;
        //cout << cnt << endl;
        if (cnt >= net.devices.size()) break;
        //break;
    }

/*    //验证
    cout<<endl;
    cout<<"test"<<endl;
    for(int i=0;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
            cout<<rowCol[i][j]<<" ";
        cout<<"/"<<endl;
    }*/
}

void bfsInitPlacementParrOmp(Net netParam,vector<vector<int>>& rowCol)  //并行bfs初步布局（OpenMP）
{
    //vector<vector<int>> rowCol;  //记录每行每列的元件id（外层vector为col，内层vector为row）
    bool* isSelected=new bool[netParam.devices.size()];  //元件是否已被安排行列号
    int cntOmp=0;  //已有多少元件已被安排行列号
    //memset(isSelected,0,sizeof(isSelected));  //isSelected是bool* 固定大小为8个字节
    memset(isSelected,0,sizeof(bool)*netParam.devices.size());

    int curCnt=0;
    vector<int> colVec;
    vector<int> curColVec;
    static omp_lock_t lock;
    omp_init_lock(&lock); // 初始化互斥锁
    #pragma omp parallel num_threads(thread_count) private(curCnt,curColVec) reduction(+:cntOmp)
    {
        //cout<<"hello from"<<omp_get_thread_num()<<endl;
        #pragma omp for
        for (int i = 0; i < netParam.devices.size(); i++) //遍历找到入度为零的（isStart==true）放到第一列
        {
            if (netParam.devices[i].d.empty() ||
                netParam.devices[i].d.size() == 1 && netParam.devices[i].d[0] == i)  //过滤不与其他元件有联系的元件
            {
                curCnt++;
                continue;
            }
            if (netParam.devices[i].isStart == 1)
            {
                curColVec.push_back(i);
                isSelected[i] = 1;
                netParam.devices[i].col = 0;
                //netParam.devices[i].row = curRow++;
                curCnt++;
            }
        }
        //cout<<"hello2 from"<<omp_get_thread_num()<<endl;
        cntOmp+=curCnt;
        omp_set_lock(&lock); //获得互斥器
        colVec.insert(colVec.end(),curColVec.begin(),curColVec.end());
        omp_unset_lock(&lock); //释放互斥器
    }
    rowCol.push_back(colVec);
/*    cout<<cntOmp<<endl;
    cout<<rowCol.size()<<endl;
    cout<<rowCol[0].size()<<endl;
    for(int i=0;i<rowCol[0].size();i++)
        cout<<rowCol[0][i]<<" ";
    cout<<endl;*/

    int curCol=0;  //目前在从第几列的元件找下一列的元件
    static omp_lock_t lock2;
    omp_init_lock(&lock2); // 初始化互斥锁2
    colVec.clear();
    #pragma omp parallel num_threads(thread_count) shared(cntOmp,curCol,rowCol,colVec) private(curCnt,curColVec)
    {
        //cout<<"scucessfully create threads"<<endl;
        while (1)
        {
            //vector<int> colVec;      //xie！！！！！
            //cout<<curCol<<" "<<rowCol[curCol].size()<<endl;
            curCnt = 0;
            curColVec.clear();
            if(omp_get_thread_num()==1)
            {
                colVec.clear();
            }
            #pragma omp barrier
            #pragma for
            for (int i = 0; i < rowCol[curCol].size(); i++)  //遍历当前列的元件
            {
                int id = rowCol[curCol][i];
                //int curRow = 0;
                for (int j = 0; j < netParam.devices[id].d.size(); j++) {
                    int id2 = netParam.devices[id].d[j];
                    if (isSelected[id2]) continue;
                    curCnt++;
                    curColVec.push_back(id2);
                    isSelected[id2] = true;
                    netParam.devices[id2].col = curCol + 1;
                    //netParam.devices[id2].row = curRow++;
                }
            }
            //cout<<"curCnt:"<<curCnt<<endl;
            //cntOmp+=curCnt;
            omp_set_lock(&lock2); //获得互斥器2
            colVec.insert(colVec.end(),curColVec.begin(),curColVec.end());
            cntOmp+=curCnt;
            omp_unset_lock(&lock2); //释放互斥器2
            #pragma omp barrier
            if(omp_get_thread_num()==1)
            {
                curCol++;   //只让主线程进行此操作，否则curCol将会加thread_num次
                rowCol.push_back(colVec);
                //cout<<"cntOmp:"<<cntOmp<<endl;
                colVec.clear();
                if (cntOmp >= netParam.devices.size()) break;
            }
            //cout<<netParam.devices.size()<<endl;
            //break;
            if (cntOmp >= netParam.devices.size()) break;
        }
    }

/*    //验证
    cout<<endl;
    cout<<"test"<<endl;
    for(int i=0;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
            cout<<rowCol[i][j]<<" ";
        cout<<"/"<<endl;
    }*/
}



void valuePropagationOptiPlc(Net& netParam, vector<vector<int>> &rowCol)  //值传播优化列内布局
{
    if(rowCol.size()==1) return;  //若只有一列，没有值传播调整的必要，直接返回
    //为第一列赋初值
    int scaler=1000;  //初值间隔系数
    for(int i=0;i<rowCol[0].size();i++)
    {
        netParam.devices[rowCol[0][i]].row=i;
        netParam.devices[rowCol[0][i]].value=i*scaler;
    }
    //对后面列通过值传播进行调整
    for(int i=1;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
        {
            //netParam.devices[rowCol[i][j]].row=j;
            int value=0;
            int cnt=0;
            for(int k=0;k < netParam.devices[rowCol[i][j]].d.size();k++)
            {
                int id=netParam.devices[rowCol[i][j]].d[k];
                if(netParam.devices[id].col==i-1)  //只考虑与前一列元件的连接
                {
                    cnt++;
                    value+=netParam.devices[id].value;
                }
            }
            netParam.devices[rowCol[i][j]].value=value/cnt;
        }
        //sort(rowCol[i].begin(),rowCol[i].end(),cmp1(netParam));
        //bubbleSort(netParam,rowCol[i]);
        quickSort(netParam,rowCol[i],0,rowCol[i].size()-1);
        for(int j=0;j<rowCol[i].size();j++) netParam.devices[rowCol[i][j]].row=j;
    }

    /*//验证
    for(int i=0;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
        {
            cout<<rowCol[i][j]<<"("<<netParam.devices[rowCol[i][j]].value<<")    ";
        }
        cout<<endl;
    }*/
}

void valuePropagationOptiPlcOmpDyna(Net& netParam, vector<vector<int>> &rowCol)  //值传播优化列内布局(OpenMP) 动态任务分配
{
    if(rowCol.size()==1) return;  //若只有一列，没有值传播调整的必要，直接返回
    //为第一列赋初值
    int scaler=1000;  //初值间隔系数
    #pragma omp parallel num_threads(thread_count)
    {
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < rowCol[0].size(); i++)
        {
            netParam.devices[rowCol[0][i]].row = i;
            netParam.devices[rowCol[0][i]].value = i * scaler;
        }
        //对后面列通过值传播进行调整
        for (int i = 1; i < rowCol.size(); i++)
        {
            #pragma omp for schedule(dynamic)
            for (int j = 0; j < rowCol[i].size(); j++)
            {
                //netParam.devices[rowCol[i][j]].row=j;
                int value = 0;
                int cnt = 0;
                for (int k = 0; k < netParam.devices[rowCol[i][j]].d.size(); k++)
                {
                    int id = netParam.devices[rowCol[i][j]].d[k];
                    if (netParam.devices[id].col == i - 1)  //只考虑与前一列元件的连接
                    {
                        cnt++;
                        value += netParam.devices[id].value;
                    }
                }
                netParam.devices[rowCol[i][j]].value = value / cnt;
            }
            //sort(rowCol[i].begin(),rowCol[i].end(),cmp1(netParam));
            //bubbleSort(netParam,rowCol[i]);
            //quickSort(netParam,rowCol[i],0,rowCol[i].size()-1);
            //for(int j=0;j<rowCol[i].size();j++) netParam.devices[rowCol[i][j]].row=j;
        }
        //为并行加速将排序单独拿出来
        #pragma omp for schedule(dynamic)
        for (int i = 1; i < rowCol.size(); i++)
        {
            quickSort(netParam, rowCol[i], 0, rowCol[i].size() - 1);
            for (int j = 0; j < rowCol[i].size(); j++) netParam.devices[rowCol[i][j]].row = j;
        }
    }

    //验证
    /*for(int i=0;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
        {
            cout<<rowCol[i][j]<<"("<<netParam.devices[rowCol[i][j]].value<<")    ";
        }
        cout<<endl;
    }*/
}

void valuePropagationOptiPlcOmp(Net& netParam, vector<vector<int>> &rowCol)  //值传播优化列内布局(OpenMP)
{
    if(rowCol.size()==1) return;  //若只有一列，没有值传播调整的必要，直接返回
    //为第一列赋初值
    int scaler=1000;  //初值间隔系数
#pragma omp parallel num_threads(thread_count)
    {
#pragma omp for
        for (int i = 0; i < rowCol[0].size(); i++)
        {
            netParam.devices[rowCol[0][i]].row = i;
            netParam.devices[rowCol[0][i]].value = i * scaler;
        }
        //对后面列通过值传播进行调整
        for (int i = 1; i < rowCol.size(); i++)
        {
#pragma omp for
            for (int j = 0; j < rowCol[i].size(); j++)
            {
                //netParam.devices[rowCol[i][j]].row=j;
                int value = 0;
                int cnt = 0;
                for (int k = 0; k < netParam.devices[rowCol[i][j]].d.size(); k++)
                {
                    int id = netParam.devices[rowCol[i][j]].d[k];
                    if (netParam.devices[id].col == i - 1)  //只考虑与前一列元件的连接
                    {
                        cnt++;
                        value += netParam.devices[id].value;
                    }
                }
                netParam.devices[rowCol[i][j]].value = value / cnt;
            }
            //sort(rowCol[i].begin(),rowCol[i].end(),cmp1(netParam));
            //bubbleSort(netParam,rowCol[i]);
            //quickSort(netParam,rowCol[i],0,rowCol[i].size()-1);
            //for(int j=0;j<rowCol[i].size();j++) netParam.devices[rowCol[i][j]].row=j;
        }
        //为并行加速将排序单独拿出来
#pragma omp for
        for (int i = 1; i < rowCol.size(); i++)
        {
            quickSort(netParam, rowCol[i], 0, rowCol[i].size() - 1);
            for (int j = 0; j < rowCol[i].size(); j++) netParam.devices[rowCol[i][j]].row = j;
        }
    }

    //验证
    /*for(int i=0;i<rowCol.size();i++)
    {
        for(int j=0;j<rowCol[i].size();j++)
        {
            cout<<rowCol[i][j]<<"("<<netParam.devices[rowCol[i][j]].value<<")    ";
        }
        cout<<endl;
    }*/
}

bool cmp1(int i,int j,Net netParam)
{
    return netParam.devices[i].value<netParam.devices[j].value;
}

void bubbleSort(Net& netParam,vector<int>&curCol)
{
    cout<<curCol.size()<<endl;
    for(int i=0;i<curCol.size();i++)
    {
        for(int j=0;j<curCol.size()-i-1;j++)
        {
            if(netParam.devices[curCol[j]].value>netParam.devices[curCol[j+1]].value)
            {
                int temp=curCol[j];
                curCol[j]=curCol[j+1];
                curCol[j+1]=temp;
            }
        }
    }
}

void quickSort(Net& netParam,vector<int>&curCol,int a,int b)
{
    if(a>=b) return;
    double midValue=netParam.devices[curCol[a]].value;
    int il=a,ir=b;
    while(il!=ir)
    {
        while(netParam.devices[curCol[ir]].value>=midValue && il<ir) ir--;  //一定要右边的先移动
        while(netParam.devices[curCol[il]].value<=midValue && il<ir) il++;
        //while(netParam.devices[curCol[ir]].value>=midValue && il<ir) ir--;
        if(il<ir)
        {
            int temp = curCol[il];
            curCol[il] = curCol[ir];
            curCol[ir] = temp;
        }
    }
    int temp=curCol[a];
    curCol[a]=curCol[ir];
    curCol[ir]=temp;
    quickSort(netParam,curCol,a,il-1);
    quickSort(netParam,curCol,ir+1,b);
}
