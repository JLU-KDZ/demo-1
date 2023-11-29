#ifndef ITEM_H
#define ITEM_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct item {
    char brand[40];//品牌
    char sonBrand[40];//子品牌
    int volume;//容量（毫升/件）
    int size;//规格（件/箱）
    long long price;//单价（元/件）
    struct item* next;//此处指针仅用于零售
};

struct itemNode {
    struct item item0;
    long long amount=0;//数量（箱）
    struct itemNode* next;
};

typedef struct itemNode* pItemNode;

extern pItemNode rootItem;
extern pItemNode tempPtr0;
extern pItemNode tempPtr;
extern struct item* rootRetail;
extern int flagRetail;

int itemInit(void);//商品初始化，生成商品链表
int itemIn(void);//进货
int itemOut(void) ;//出货
void insertItem(pItemNode p);//将商品结点插入到商品链表中
pItemNode searchItem(pItemNode p);//在商品链表中查找相应的商品
void deleteItem(pItemNode p);//删除商品链表中的某一结点
void removeItem(pItemNode head);//释放商品链表中所有结点
void insertRetail(struct item* p);//将商品结点插入到零售商品链表中
int searchRetail(struct item* p);//在零售商品链表中查找相应的商品
void deleteRetail(struct item* p,int n);//删除零售商品链表中的某一结点

#endif // ITEM_H
