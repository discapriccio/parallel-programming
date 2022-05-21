#include<sys/time.h>
#include <iostream>
#include <fstream>
#include "string.h"
#include<ctime>
#include"../head/Parser.h"
#include"../head/Net.h"
#include"../head/Placement.h"
#include"TimeRecord.h"

int main() {
    string path = "/home/s2012197/openmp/ASG_arm/source/34696.spi";
    vector<Net> netVector;

    vector<vector<int>> rowCol;
    int cir=1;
    struct timeval tv_begin, tv_end;
    unsigned long long diff;

    TimeRecord<chrono::milliseconds> totalProcess("totalProcess");

    //gettimeofday(&tv_begin, NULL);
    totalProcess.startRecord();
    parser(path, netVector);
    //gettimeofday(&tv_end, NULL);
    //unsigned long long diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    //cout << "time for parser:"<<(diff / 1000.0) / cir << "ms" << endl;
    totalProcess.endRecord();
    cout<<"time for parser"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();


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


    //gettimeofday(&tv_begin, NULL);
    totalProcess.startRecord();
    for(int i=0;i<cir;i++)
        bfsInitPlacement(netVector[tm],rowCol);
    //gettimeofday(&tv_end, NULL);
    //diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    //cout << "time for chuanxing bfs:"<<(diff / 1000.0) / cir << "ms" << endl;
    totalProcess.endRecord();
    cout<<"time for chuanxing bfs"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();
    for(int i=0;i<rowCol.size();i++)
    {
        cout<<rowCol[i].size()<<endl;
    }


    /*gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        bfsInitPlacementParr(netVector[tm]);
    gettimeofday(&tv_end, NULL);
    diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    cout << "time for bfs parallel pthread:"<<(diff / 1000.0) / cir << "ms" << endl;

    vector<vector<int>> emp;
    rowCol.clear();
    rowCol.swap(emp);
    

    gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        bfsInitPlacementParrOmp(netVector[tm],rowCol);
    gettimeofday(&tv_end, NULL);
    diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    cout << "time for bfs parallel omp:"<<(diff / 1000.0) / cir << "ms" << endl;
*/
	

    totalProcess.startRecord();
	//gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        valuePropagationOptiPlcOmp(netVector[tm],rowCol);
    //gettimeofday(&tv_end, NULL);
    //diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    //cout << "time for vp chuanxing:"<<(diff / 1000.0) / cir << "ms" << endl;
    totalProcess.endRecord();
    cout<<"time for vp omp"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();
    for(int i=0;i<rowCol.size();i++)
    {
	cout<<rowCol[i].size()<<endl;
    }
    
    vector<vector<int>> emp;
    rowCol.clear();
    rowCol.swap(emp);
    totalProcess.startRecord();
    for(int i=0;i<cir;i++)
        bfsInitPlacement(netVector[tm],rowCol);
    totalProcess.endRecord();
    cout<<"time for chuanxing bfs"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();


    totalProcess.startRecord();	
    //gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        valuePropagationOptiPlcOmpDyna(netVector[tm],rowCol);
    //gettimeofday(&tv_end, NULL);
    //diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    //cout << "time for vp omp:"<<(diff / 1000.0) / cir << "ms" << endl;
    totalProcess.endRecord();
    cout<<"time for vp omp dyna"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();    
    for(int i=0;i<rowCol.size();i++)
    {
        cout<<rowCol[i].size()<<endl;
    }


    //vector<vector<int>> emp;
    rowCol.clear();
    rowCol.swap(emp);
    totalProcess.startRecord();
    for(int i=0;i<cir;i++)
        bfsInitPlacement(netVector[tm],rowCol);
    totalProcess.endRecord();
    cout<<"time for chuanxing bfs"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();


    totalProcess.startRecord();	
    //gettimeofday(&tv_begin, NULL);
    for(int i=0;i<cir;i++)
        valuePropagationOptiPlc(netVector[tm],rowCol);
    //gettimeofday(&tv_end, NULL);
    //diff = 1000000 * (tv_end.tv_sec - tv_begin.tv_sec) + tv_end.tv_usec - tv_begin.tv_usec;
    //cout << "time for vp omp dyna:"<<(diff / 1000.0) / cir << "ms" << endl;
    totalProcess.endRecord();
    cout<<"time for vp chuanxing"<<endl;
    totalProcess.print();
    cout<<endl;
    totalProcess.clearRecord();
    for(int i=0;i<rowCol.size();i++)
    {
        cout<<rowCol[i].size()<<endl;
    }


    return 1;
}
