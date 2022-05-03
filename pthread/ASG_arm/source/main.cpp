#include<sys/time.h>
#include <iostream>
#include <fstream>
#include "string.h"
#include<ctime>
#include"../head/Parser.h"
#include"../head/Net.h"
#include"../head/Placement.h"

int main() {
    string path = "34696.spi";
    vector<Net> netVector;

    int cir=1;
    struct timeval tv_begin, tv_end;
    gettimeofday(&tv_begin, NULL);
    parser(path, netVector);
    gettimeofday(&tv_end, NULL);
    unsigned long long diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    cout << "time for parser:"<<(diff / 1000.0) / cir << "ms" << endl;

    int tm=0;

    //cout<<"device num:"<<netVector[tm].devices.size()<<endl;
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
    gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        bfsInitPlacement(netVector[tm]);
    gettimeofday(&tv_end, NULL);
    diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    cout << "time for chuanxing:"<<(diff / 1000.0) / cir << "ms" << endl;

    gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        bfsInitPlacementParr(netVector[tm]);
    gettimeofday(&tv_end, NULL);
    diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    cout << "time for parallel:"<<(diff / 1000.0) / cir << "ms" << endl;
    return 1;
}
