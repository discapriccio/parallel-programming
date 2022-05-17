//
// Created by xingyu on 2022/4/30.
//
//布局函数
#include"Net.h"
#include"Parser.h"
#include <string.h>
#include<vector>
#include<pthread.h>
#include<omp.h>
#include<algorithm>
#include<cmath>
#ifndef ASG_PLACEMENT_H
#define ASG_PLACEMENT_H


void bfsInitPlacement(Net& net,vector<vector<int>>& rowCol);  //bfs初步布局
void bfsInitPlacementParr(Net netParam);  //并行bfs初步布局（Pthread）
void bfsInitPlacementParrOmp(Net netParam);  //并行bfs初步布局（OpenMP）

void valuePropagationOptiPlc(Net& netParam, vector<vector<int>>& rowCol);  //值传播优化列内布局
bool cmp1(int i,int j,Net netParam);
void bubbleSort(Net& netParam,vector<int>&curCol);
void quickSort(Net& netParam,vector<int>&curCol,int a,int b);
#endif //ASG_PLACEMENT_H