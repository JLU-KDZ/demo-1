#ifndef CUTOMER_H
#define CUTOMER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*规定客户分为5级，其对应的折扣为：零级用户：原价/一级客户：九折/二级客户：八折/三级客户：七折/四级用户：六折*/
struct customer {
    char name[30];//客户名称
    int level=0;//客户等级
    struct customer* next;
};

typedef struct customer* pCustomer;
extern pCustomer rootCustomer;

int customerInit(void);//客户初始化，生成客户链表
int customerIn(void);//添加客户
void insertCustomer(pCustomer p);//在客户链表中插入新结点
int searchCustomer(pCustomer p);//在客户链表中搜索某一客户，并返回客户的等级
void changeCustomer(char* customerName,int customerLevel);//修改客户链表中某客户的等级
void customerBubbleSort(pCustomer head);//对链表冒泡排序，级别高的在前
void removeCustomer(pCustomer head);//释放客户链表中所有结点的内存

#endif // CUTOMER_H
