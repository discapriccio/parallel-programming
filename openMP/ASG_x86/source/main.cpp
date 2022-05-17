#include <windows.h>
#include <iostream>
#include"../head/Parser.h"
#include"../head/Net.h"
#include"../head/Placement.h"

int main() {
    string path = "ram2.spi";
    vector<Net> netVector;
    LARGE_INTEGER tm1,tm2,tmc;
    QueryPerformanceFrequency(&tmc);
    QueryPerformanceCounter(&tm1);

    parser(path, netVector);
    QueryPerformanceCounter(&tm2);
    double time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
    cout<<"Time for parser (QueryPerformanceCounter):"<<time*1000<<"ms"<<endl<<endl;
    cout<<netVector.size()<<endl;

    int tm=0;
    cout<<"device num:"<<netVector[tm].devices.size()<<endl;
/*    //验证
    cout<<"device num:"<<netVector[tm].devices.size()<<endl;
    cout<<"stream num:"<<netVector[tm].streams.size()<<endl;
    for(int i=0;i<netVector[tm].streams.size();i++)
        cout<<netVector[tm].streams[i].name<<" ";
    cout<<endl;
    cout<<"strings connected to certain devices:"<<endl;  //验证与特定元件连接的线是否正确读入数据结构
    for(int i=0;i<netVector[tm].devices.size();i++)
    {
        cout<<netVector[tm].devices[i].name<<":"<<endl;
        for(int j=0;j<netVector[tm].devices[i].s.size();j++)
            cout<<netVector[tm].streams[netVector[tm].devices[i].s[j]].name<<" ";
        cout<<endl;
    }
    cout<<"devices connected to certain strings:"<<endl; //验证与特定线连接的元件是否正确读入数据结构
    for(int i=0;i<netVector[tm].streams.size();i++)
    {
        cout<<netVector[tm].streams[i].name<<":"<<endl;
        for(int j=0;j<netVector[tm].streams[i].d.size();j++)
        {
            cout<<netVector[tm].devices[netVector[tm].streams[i].d[j]].name<<" ";
        }
        cout<<endl;
    }
    cout<<"devices connected to certain devices:"<<endl; //验证与特定元件连接的元件是否正确读入数据结构
    for(int i=0;i<netVector[tm].devices.size();i++)
    {
        cout<<netVector[tm].devices[i].name<<":"<<endl;
        cout<<"size: "<<netVector[tm].devices[i].d.size()<<endl;
        for(int j=0;j<netVector[tm].devices[i].d.size();j++)
        {
            cout<<netVector[tm].devices[i].d[j]<<" ";
        }
        cout<<endl;
    }*/
    vector<vector<int>> rowCol;
    int cir=1;
    QueryPerformanceFrequency(&tmc);
    QueryPerformanceCounter(&tm1);
    for(int i=0;i<cir;i++)
        bfsInitPlacement(netVector[tm],rowCol);  //串行bfs初步布局
    QueryPerformanceCounter(&tm2);
    time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
    cout<<"chuanxing (QueryPerformanceCounter):"<<time*1000/cir<<"ms"<<endl<<endl;

/*    QueryPerformanceFrequency(&tmc);
    QueryPerformanceCounter(&tm1);
    for(int i=0;i<cir;i++)
        bfsInitPlacementParr(netVector[tm]);  //并行bfs初步布局
    QueryPerformanceCounter(&tm2);
    time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
    cout<<"parallel pthread (QueryPerformanceCounter):"<<time*1000/cir<<"ms"<<endl<<endl;

     QueryPerformanceFrequency(&tmc);
     QueryPerformanceCounter(&tm1);
     for(int i=0;i<cir;i++)
         bfsInitPlacementParrOmp(netVector[tm]);  //并行bfs初步布局（OPenMP）
     QueryPerformanceCounter(&tm2);
     time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
     cout<<"parallel OpenMP (QueryPerformanceCounter):"<<time*1000/cir<<"ms"<<endl<<endl;*/

    QueryPerformanceFrequency(&tmc);
    QueryPerformanceCounter(&tm1);
    for(int i=0;i<cir;i++)
        valuePropagationOptiPlc(netVector[tm],rowCol);
    QueryPerformanceCounter(&tm2);
    time=(double)(tm2.QuadPart-tm1.QuadPart)/(double)tmc.QuadPart;
    cout<<"value propagation optimize placement (QueryPerformanceCounter):"<<time*1000/cir<<"ms"<<endl<<endl;
    return 1;
}
