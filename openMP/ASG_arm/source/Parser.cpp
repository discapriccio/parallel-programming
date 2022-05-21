/*
//
// Created by xingyu on 2022/4/20.
//
//目前只考虑M（mos管）和X（组合型）两种元件，将文件读入新的数据结构中
//尚未处理连接到此电路外面的线  --> 目前假设连接到此电路外面的线均是输入此电路的
//尚未过滤只和一个原件相连，且不是连接到电路外面的线

#include"../head/Parser.h"
#include<algorithm>
using namespace std;

void parser(string path, vector<Net>&v)
{
    ifstream inFile;
    inFile.open(path.c_str(), ios::in);
    string tmp;
    char a,b;
    while(inFile>>tmp)  //读入文件
    {
        if (tmp == ".subckt" || tmp == ".SUBCKT")
        {
            Net n;
            inFile>>n.netName;
            int cur11;
            while (true)   //读入表头
            {
                inFile >> tmp;
                if (tmp[0] == '+')  //+代表换行
                    tmp.erase(0, 1);  //把+号去掉
                if (tmp.find('=') != tmp.npos)  //有“=”
                {
                    getline(inFile, tmp);  //？
                    break;
                }
                cur11 = inFile.tellg();
                inFile.get(a);
                inFile.get(b);
                if (a == '\n' && b != '+')
                {
                    Stream s;
                    s.name=tmp;
                    s.isIn=true;
                    n.streams.push_back(s);
                    n.stream2ind.insert(pair<string, int> (tmp, n.streams.size()-1));
                    inFile.seekg(cur11, ios_base::beg);
                    break;
                }
                inFile.seekg(cur11, ios_base::beg);    //ios_base::beg：相对于文件开始处的偏移量
                if (tmp != "/")
                {
                    Stream s;
                    s.name=tmp;
                    s.isIn=true;
                    n.streams.push_back(s);
                    n.stream2ind.insert(pair<string, int> (tmp, n.streams.size()-1));
                }
            }
           */
/* //将与此电路外连线的前一半认定为in，后一半认定为out
            for(int i=n.streams.size()/2;i<n.streams.size();i++)
            {
                n.streams[i].isIn=0;
                n.streams[i].isOut=1;
            }*//*

            inFile >> tmp;
            string tmp11 = tmp;
            cur11 = inFile.tellg();
            while (tmp != ".ends" && tmp != ".ENDS")  //读入每个元件和其对应连线
            {
                if (tmp[0] == 'M')  //MOS管
                {
                    Device d;
                    d.name=tmp;
                    n.devices.push_back(d);
                    int id=n.devices.size()- 1;
                    n.dev2ind.insert(pair<string, int> (tmp, id));
                    for (int i = 0; i < 4; i++)  //读入四条线
                    {
                        inFile >> tmp;
                        Stream s;
                        s.name=tmp;
                        if (n.stream2ind.insert(pair<string, int> (tmp,n.streams.size())).second) //若成功插入（不与之前的重复）
                        {
                            n.streams.push_back(s);
                        }
                        n.devices[id].s.push_back(n.stream2ind[tmp]);
                        //n.streams[n.streams.size()-1].d.push_back(n.dev2ind[d.name]);
                        vector<int> v=n.streams[n.stream2ind[tmp]].d;
                        //if(find(v.begin(),v.end(),n.dev2ind[d.name])==v.end())  //避免重复加入
                            n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);
                    }
                }
                else if (tmp[0] == 'X')  //组合型
                {
                    Device d;
                    d.name=tmp;
                    n.devices.push_back(d);
                    n.dev2ind.insert(pair<string, int> (tmp, n.devices.size()- 1));

                    inFile >> tmp;
                    int cur = inFile.tellg();
                    string tmp1,tmp2;

                    inFile.get(a);
                    inFile >> tmp1;//pre-read subcircuit name
                    inFile >> tmp2;//pre-read subcircuit feature
                    if ((a != '\n') && (tmp2.find('=') == tmp2.npos))  // deal with conditions that there exist no-interface device
                    {
                        do {
                            if (tmp[0] == '+')//deal with the condition that '+' prolong the rank
                                tmp.erase(0, 1);
                            Stream s;
                            s.name = tmp;
                            if (n.stream2ind.insert(pair<string, int>(tmp, n.streams.size())).second)
                                n.streams.push_back(s);
                            n.devices[n.devices.size()- 1].s.push_back(n.stream2ind[tmp]);
                            //n.streams[n.streams.size()-1].d.push_back(n.dev2ind[d.name]);
                            vector<int> v=n.streams[n.stream2ind[tmp]].d;
                            //if(find(v.begin(),v.end(),n.dev2ind[d.name])==v.end())  //避免重复加入
                                n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);

                            tmp = tmp1;
                            tmp1 = tmp2;
                            cur = inFile.tellg();
                            inFile.get(a);
                            inFile.get(b);
                            if (a == '\n' && b != '+') {
                                s.name = tmp;
                                if (n.stream2ind.insert(pair<string, int>(tmp, n.streams.size())).second)
                                    n.streams.push_back(s);
                                n.devices[n.devices.size()- 1].s.push_back(n.stream2ind[tmp]);
                                //n.streams[n.streams.size()-1].d.push_back(n.dev2ind[d.name]);
                                vector<int> v=n.streams[n.stream2ind[tmp]].d;
                                //if(find(v.begin(),v.end(),n.dev2ind[d.name])==v.end())  //避免重复加入
                                    n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);
                                break;
                            }
                            inFile.seekg(cur, ios_base::beg);
                            inFile >> tmp2;
                            if (tmp2.find('=') != tmp2.npos) {
                                s.name = tmp;
                                if (n.stream2ind.insert(pair<string, int>(tmp, n.streams.size())).second)
                                    n.streams.push_back(s);
                                n.devices[n.devices.size()- 1].s.push_back(n.stream2ind[tmp]);
                                vector<int> v=n.streams[n.stream2ind[tmp]].d;
                                //if(find(v.begin(),v.end(),n.dev2ind[d.name])==v.end())  //避免重复加入
                                    n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);
                                break;
                            }
                        } while (true);
                    }
                    inFile.seekg(cur, ios_base::beg);// give back the pre-read
                }
                getline(inFile, tmp);
                inFile >> tmp;
            }

            //预处理，填充device-device
            for(int i=0;i<n.streams.size();i++)
            {
                //标注入度为零的元件
                if(n.streams[i].isIn==1)
                {
                    for(int j=0;j<n.streams[i].d.size();j++)
                    {
                        n.devices[n.streams[i].d[j]].isStart=1;
                    }
                }
                for(int j=0;j<n.streams[i].d.size();j++)
                {
                    for(int k=0;k<n.streams[i].d.size();k++)
                    {
                        if(k==j) continue;
                        vector<int> v=n.devices[n.streams[i].d[j]].d;
                        //if(find(v.begin(),v.end(),n.streams[i].d[k])==v.end())  //避免重复
                            n.devices[n.streams[i].d[j]].d.push_back(n.streams[i].d[k]);
                    }
                }
                //标记不与其他元件连线的元件
                //for(int i=0;i
            }
            v.push_back(n);//push net into netVector
        }
    }

    cout<<"end parser"<<endl;
}


*/

//
// Created by xingyu on 2022/4/20.
//
//目前只考虑M（mos管）和X（组合型）两种元件，将文件读入新的数据结构中
//尚未处理连接到此电路外面的线  --> 目前假设连接到此电路外面的线均是输入此电路的
//尚未过滤只和一个原件相连，且不是连接到电路外面的线

#include"../head/Parser.h"
using namespace std;

void parser(string path, vector<Net>&v)
{
    ifstream inFile;
    inFile.open(path.c_str(), ios::in);
    string tmp;
    char a,b;
    while(inFile>>tmp)  //读入文件
    {
        if (tmp == ".subckt" || tmp == ".SUBCKT")
        {
            Net n;
            inFile>>n.netName;
            int cur11;
            while (true)   //读入表头
            {
                inFile >> tmp;
                if (tmp[0] == '+')  //+代表换行
                    tmp.erase(0, 1);  //把+号去掉
                if (tmp.find('=') != tmp.npos)  //有“=”
                {
                    getline(inFile, tmp);  //？
                    break;
                }
                cur11 = inFile.tellg();
                inFile.get(a);
                inFile.get(b);
                if (a == '\n' && b != '+')
                {
		    //cout<<"\n + 1"<<endl;
                    Stream s;
                    s.name=tmp;
                    s.isIn=true;
                    n.streams.push_back(s);
                    n.stream2ind.insert(pair<string, int> (tmp, n.streams.size()-1));
                    inFile.seekg(cur11, ios_base::beg);
                    break;
                }
                inFile.seekg(cur11, ios_base::beg);    //ios_base::beg：相对于文件开始处的偏移量
                if (tmp != "/")
                {
                    Stream s;
                    s.name=tmp;
                    s.isIn=true;
                    n.streams.push_back(s);
                    n.stream2ind.insert(pair<string, int> (tmp, n.streams.size()-1));
                }
            }
	    //将与此电路连线的前一半认定为in，后一半认定为out
	    for(int i=n.streams.size()/2;i<n.streams.size();i++)
	    {
		n.streams[i].isIn=0;
		n.streams[i].isOut=1;
	    }
            inFile >> tmp;
            string tmp11 = tmp;
            cur11 = inFile.tellg();
            while (tmp != ".ends" && tmp != ".ENDS")  //读入每个元件和其对应连线
            {
                if (tmp[0] == 'M')  //MOS管
                {
                    Device d;
                    d.name=tmp;
                    n.devices.push_back(d);
		    //cout<<n.devices.size()<<endl;
                    int id=n.devices.size()- 1;
                    n.dev2ind.insert(pair<string, int> (tmp, id));
                    for (int i = 0; i < 4; i++)  //读入四条线
                    {
                        inFile >> tmp;
                        Stream s;
                        s.name=tmp;
                        if (n.stream2ind.insert(pair<string, int> (tmp,n.streams.size())).second) //若成功插入（不与之前的重复）
                        {
                            n.streams.push_back(s);
                        }
                        n.devices[id].s.push_back(n.stream2ind[tmp]);
                        //n.streams[n.streams.size()-1].d.push_back(n.dev2ind[d.name]);
                        n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);
                    }
                }
                else if (tmp[0] == 'X')  //组合型
                {
                    Device d;
                    d.name=tmp;
                    n.devices.push_back(d);
		    //cout<<n.devices.size()<<endl;
                    n.dev2ind.insert(pair<string, int> (tmp, n.devices.size()- 1));

                    inFile >> tmp;
                    int cur = inFile.tellg();
                    string tmp1,tmp2;

                    inFile.get(a);
                    inFile >> tmp1;//pre-read subcircuit name
                    inFile >> tmp2;//pre-read subcircuit feature
                    if ((a != '\n') && (tmp2.find('=') == tmp2.npos))  // deal with conditions that there exist no-interface device
                    {
                        do {
                            if (tmp[0] == '+')//deal with the condition that '+' prolong the rank
                                tmp.erase(0, 1);
                            Stream s;
                            s.name = tmp;
                            if (n.stream2ind.insert(pair<string, int>(tmp, n.streams.size())).second)
                                n.streams.push_back(s);
                            n.devices[n.devices.size()- 1].s.push_back(n.stream2ind[tmp]);
                            //n.streams[n.streams.size()-1].d.push_back(n.dev2ind[d.name]);
                            n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);

                            tmp = tmp1;
                            tmp1 = tmp2;
                            cur = inFile.tellg();
                            inFile.get(a);
                            inFile.get(b);
                            //if (a == '\n' && b != '+')
                            //if(a=='\n'&& b!=48) 
                            if(a=='\r'&&b!='+'){
				//cout<<"get in +"<<endl;
                                s.name = tmp;
                                if (n.stream2ind.insert(pair<string, int>(tmp, n.streams.size())).second)
                                    n.streams.push_back(s);
                                n.devices[n.devices.size()- 1].s.push_back(n.stream2ind[tmp]);
                                //n.streams[n.streams.size()-1].d.push_back(n.dev2ind[d.name]);
                                n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);
                                break;
                            }
                            inFile.seekg(cur, ios_base::beg);
                            inFile >> tmp2;
                            if (tmp2.find('=') != tmp2.npos) {
                                s.name = tmp;
                                if (n.stream2ind.insert(pair<string, int>(tmp, n.streams.size())).second)
                                    n.streams.push_back(s);
                                n.devices[n.devices.size()- 1].s.push_back(n.stream2ind[tmp]);
                                n.streams[n.stream2ind[tmp]].d.push_back(n.dev2ind[d.name]);
                                break;
                            }
                        } while (true);
                    }
                    inFile.seekg(cur, ios_base::beg);// give back the pre-read
                }
                getline(inFile, tmp);
                inFile >> tmp;
            }
	    cout<<"read in phrase 1 done"<<endl;

            //预处理，填充device-device
            for(int i=0;i<n.streams.size();i++)
            {
                //标注入度为零的元件
                if(n.streams[i].isIn==1)
                {
                    for(int j=0;j<n.streams[i].d.size();j++)
                    {
                        n.devices[n.streams[i].d[j]].isStart=1;
                    }
                }
                for(int j=0;j<n.streams[i].d.size();j++)
                {
                    for(int k=0;k<n.streams[i].d.size();k++)
                    {
                        if(k==j) continue;
                        n.devices[n.streams[i].d[j]].d.push_back(n.streams[i].d[k]);
                    }
                }
                //标记不与其他元件连线的元件
                //for(int i=0;i
            }
            v.push_back(n);//push net into netVector
        }
    }

    cout<<"end parser"<<endl;
}



