//
// Created by xingyu on 2022/4/30.
//
#include<cstring>
#include<string>
#include<vector>
#include<map>
using namespace std;
#ifndef ASG_NET_H
#define ASG_NET_H

class Device{
public:
    string name;  //元件名称
    vector<int> s;  //与此元件连接的线
    vector<int> d;  //与此元件连接的元件
    bool isStart;  //是否是入度为零的元件
    int col;  //所在列号
    int row;  //所在行号
    bool isBad;  //是否是不和其他元件连接的元件
};

class Stream{
public:
    string name; //线名称
    vector<int> d;  //与此线连接的元件
    bool isIn;  //是否是总输入线
    bool isOut;  //是否是总输出线
};
class Net {
public:
    vector<Device> devices;
    vector<Stream> streams;
    map<string, int> dev2ind;
    map<string, int> stream2ind;
    string netName;
};


#endif //ASG_NET_H
