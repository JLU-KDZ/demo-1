#ifndef CALCULATE_H
#define CALCULATE_H

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "item.h"
#include "customer.h"

struct linkNode {
    char brand[40];//品牌
    long long amount;//数量
    struct linkNode* next;
};//用于回归计算的链表结点

typedef struct linkNode node;

long long cost(void);//计算进货成本
long long revenue(const char* name);//计算出货收入
void insertNode (node* p,int k);//将结点从链表尾插入链表
void removeNode(struct linkNode* head);//释放链表内所有结点的内存
int getRand(int min, int max);//生成一个[min，max]区间内一个随机整数
/* 重载函数
 * 根据输入内容的不同类型，将其写入文件，并添加适当数量的\t，使数据对齐*/
void writeT(FILE* fp,const char* str);
void writeT(FILE* fp,long long num);
void writeT(FILE* fp,int num);
double back(struct linkNode* head);//线性回归计算函数

extern node* root[10];

#endif // CALCULATE_H
