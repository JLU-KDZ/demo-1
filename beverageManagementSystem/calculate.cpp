#include "calculate.h"
#include "item.h"

#define N2 200//数量大于等于N2的商品为堆积商品，八折销售

node* root[10]={NULL};//回归计算中十种酒水品牌的链表头指针

long long cost(void)
{
    FILE* fp;
    if (fopen_s(&fp, "E:\\fpInI.txt", "r") != 0) {
        return -1;//打开文件失败，返回-1传递到前端处理，下同
    }
    long long sum=0;
    pItemNode p;
    p = (pItemNode)malloc(sizeof(struct itemNode));
    if (p == NULL){
        fclose(fp);
        return -2;//动态空间申请失败，返回-2传递到前端处理
    }
    while (fscanf(fp, "%s%s%d%d%I64d%I64d", p->item0.brand, p->item0.sonBrand, &(p->item0.volume), &(p->item0.size), &(p->item0.price),&(p->amount)) != EOF)
    {
        sum = sum + p->item0.size * p->item0.price * p->amount;
    }
    free(p);
    fclose(fp);
    return sum;
}

long long revenue(const char* name)//name为客户名称
{
    FILE* fp;
    if (fopen_s(&fp, "E:\\fpOutI.txt", "r") != 0) {
        return -1;
    }
    long long sum=0;
    pItemNode p;
    p = (pItemNode)malloc(sizeof(struct itemNode));
    if (p == NULL){
        fclose(fp);
        return -2;
    }
    while (fscanf(fp, "%s%s%d%d%I64d%I64d", p->item0.brand, p->item0.sonBrand, &(p->item0.volume), &(p->item0.size), &(p->item0.price),&(p->amount)) != EOF)
    {
        pItemNode temp= searchItem(p);
        if(temp!=NULL)
        {
            if(temp->amount>=N2)
            {
                sum = sum + (p->item0.size * p->item0.price * p->amount)*0.8;//堆积商品按八折出售
            }
            else
            {
                sum = sum + (p->item0.size * p->item0.price * p->amount);
            }
        }
        tempPtr0=NULL;
        tempPtr = rootItem;
    }
    sum=sum*1.5;//在不打折情况下，售价定为进价的1.5倍
    pCustomer t;
    t = (pCustomer)malloc(sizeof(struct customer));
    if (t == NULL){
        free(p);
        fclose(fp);
        return -2;
    }
    strcpy(t->name,name);
    int level=searchCustomer(t);
    sum = sum * (1 - 0.1 * level);
    free(p);
    free(t);
    fclose(fp);
    return sum;
}

void insertNode (node* p,int k) {
    p->next=NULL;
    node* p0;
    p0 = root[k];
    if (p0 == NULL) {
        root[k] = p;
        return;
    }
    else {
        while (p0->next != NULL)
            p0 = p0->next;
        p0->next = p;
        return;
    }
}

void removeNode(struct linkNode* head){
    node* temp;
    while(head!=NULL){
        temp=head;
        free(temp);
        head=head->next;
    }
    return;
}

int getRand(int min, int max) {
    return ( rand() % (max - min + 1) ) + min ;
}

/* 长度小于6字节的字符串添加\t后，长度补至6字节
 * 长度大于等于6字节的字符串添加\t，长度补至12字节
 * 依此类推*/
void writeT(FILE* fp,const char* str){
    int len=strlen(str);
    if(len<6)
    {
        fprintf_s(fp,"%s","\t\t\t");
    }
    if(len>=6&&len<12)
    {
        fprintf_s(fp,"%s","\t\t");
    }
    if(len>=12)
    {
        fprintf_s(fp,"%s","\t");
    }
}
void writeT(FILE* fp,long long num){
    int len=0;
    while(num>0)
    {
        num=num/10;
        len++;
    }
    if(len<6)
    {
        fprintf_s(fp,"%s","\t\t\t");
    }
    if(len>=6&&len<12)
    {
        fprintf_s(fp,"%s","\t\t");
    }
    if(len>=12)
    {
        fprintf_s(fp,"%s","\t");
    }
}

void writeT(FILE* fp,int num){
    int len=0;
    while(num>0)
    {
        num=num/10;
        len++;
    }
    if(len<6)
    {
        fprintf_s(fp,"%s","\t\t\t");
    }
    if(len>=6&&len<12)
    {
        fprintf_s(fp,"%s","\t\t");
    }
    if(len>=12)
    {
        fprintf_s(fp,"%s","\t");
    }
}

double back(struct linkNode* head) {
    long long sum_x = 0, sum_y = 0, sum_xx = 0, sum_xy = 0,n=0,x=0;
    double k=0,b=0,a_x=0,a_y=0;
    double result=0;
    node* p;
    p = head;
    if(p==NULL)
        return -1;
    while (p!=NULL) {
        n++;
        x++;
        sum_x += x; // x的和
        sum_y += p->amount; // y的和
        sum_xx += x * x; // x的平方和
        sum_xy += x * (p->amount); // x和y的乘积和
        p=p->next;
    }
    a_y = sum_y /(double) n;
    a_x = sum_x / (double) n;
    k=(sum_xy-n*a_x*a_y)/(sum_xx-n*a_x*a_x);//斜率
    b=a_y-k*a_x;//截距
    result=k*(n+1)+b;//下一次的预测值
    return result;
}
