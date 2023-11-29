#include"item.h"

pItemNode rootItem=NULL;//商品链表的头指针

/*tempPtr0和tempPtr用于搜索商品节点*/
pItemNode tempPtr0=NULL;
pItemNode tempPtr=rootItem;
struct item* rootRetail=NULL;//零售商品链表的头指针
int flagRetail=1;//此处用于标记searchItem的两种用法

int itemInit(void) {
    FILE* fp;
    if (fopen_s(&fp,"E:\\fpInitI.txt", "r") !=0){
       return 1;//打开文件失败，返回1传递到前端处理，下同
    }
    pItemNode p;
    p = (pItemNode)malloc(sizeof(struct itemNode));
    if (p == NULL){
        fclose(fp);
        return 2;//动态空间申请失败，返回2传递到前端处理
    }
    while ((fscanf(fp, "%s%s%d%d%I64d%I64d", p->item0.brand, p->item0.sonBrand, &(p->item0.volume), &(p->item0.size), &(p->item0.price),&(p->amount))) != EOF) {
        insertItem(p);
        p=NULL;
        p= (pItemNode)malloc(sizeof(struct itemNode));
        if (p == NULL) {
            fclose(fp);
            return 2;//动态空间申请失败，返回2传递到前端处理
        }
    }
    free(p);
    fclose(fp);
    return 0;
}

int itemIn(void) {
    FILE* fp;
     if(fopen_s(&fp,"E:\\fpInI.txt", "r") != 0) {
       return 1;
    }
    pItemNode p;
    p = (pItemNode)malloc(sizeof(struct itemNode));
    if (p == NULL) {
        fclose(fp);
        return 2;
    }
    while (fscanf(fp, "%s%s%d%d%I64d%I64d", p->item0.brand,  p->item0.sonBrand, &(p->item0.volume), &(p->item0.size), &(p->item0.price),&(p->amount)) != EOF) {
        if (searchItem(p) == NULL){//若链表中不存在该商品，则将其插入到链表
            insertItem(p);
            p = NULL;
            p = (pItemNode)malloc(sizeof(struct itemNode));
            if (p == NULL) {
                fclose(fp);
                return 2;
            }
        }
        else {
            tempPtr->amount += p->amount;//若链表中存在该商品，修改该商品的数量
            }
        tempPtr0=NULL;
        tempPtr = rootItem;
    }
    free(p);
    fclose(fp);
    return 0;
}

int itemOut(void) {
    FILE* fp;
    if (fopen_s(&fp, "E:\\fpOutI.txt", "r")!= 0) {
        return 1;
    }
    pItemNode p;
    p = (pItemNode)malloc(sizeof(struct itemNode));
    if (p == NULL) {
       fclose(fp);
       return 2;
    }
    int flag=1;//用于判断此次销售是否成功
    while (fscanf(fp, "%s%s%d%d%I64d%I64d", p->item0.brand, p->item0.sonBrand, &(p->item0.volume), &(p->item0.size),&(p->item0.price), &(p->amount)) != EOF) {
        pItemNode ptr=searchItem(p);
        if(ptr==NULL||tempPtr->amount<p->amount)
        {
            flag=0;//库存中不存在该商品或该商品不足
            tempPtr0=NULL;
            tempPtr=rootItem;
            break;
        }
        tempPtr0=NULL;
        tempPtr=rootItem;
    }
    rewind(fp);//重新将文件指针指向文件开头
    while ((flag)&&fscanf(fp, "%s%s%d%d%I64d%I64d", p->item0.brand, p->item0.sonBrand, &(p->item0.volume), &(p->item0.size),&(p->item0.price), &(p->amount)) != EOF) {
        searchItem(p);
        tempPtr->amount -= p->amount;
        if (tempPtr->amount == 0){
            pItemNode temp=tempPtr;
            deleteItem(temp);//若某商品数量为零，从链表中将该商品删除
        }
        tempPtr0 = NULL;
        tempPtr = rootItem;
    }
    if(flag==0)
    {
        fclose(fp);
        return 3;//销售失败，返回3传递到前端处理
    }
    fclose(fp);
    return 0;
}

void insertItem(pItemNode p) {
    p->next=NULL;
    pItemNode p0;
    p0 = rootItem;
    if (p0 == NULL) {
        rootItem = p;
        return;
    }
    else {
        while (p0->next != NULL)
            p0 = p0->next;
        p0->next = p;
        return;
    }
}

pItemNode searchItem(pItemNode p) {
    pItemNode t0,t;
    t0 = NULL;
    t = rootItem;
    /*当用于搜索零售商品时，无需使用size，令flagRetail为0*/
    while ((t != NULL&&(strcmp(p->item0.brand,t->item0.brand)!=0|| strcmp(p->item0.sonBrand, t->item0.sonBrand) != 0||(p->item0.volume!=t->item0.volume)||((p->item0.size!=t->item0.size)&&flagRetail)))) {
        t0 = t;
        t = t->next;
    }
    tempPtr0 = t0;
    tempPtr = t;
    return tempPtr;
}

void deleteItem(pItemNode p) {
    pItemNode t;
    if (p== rootItem) {

        t = p;
        p=p->next;
        free(t);
        rootItem = p;
    }
    else
    {
        p = p->next;
        free(tempPtr0->next);
        tempPtr0->next = p;
    }
    return;
}

void removeItem(pItemNode head){
    pItemNode temp;
    while(head!=NULL){
        temp=head;
        free(temp);
        head=head->next;
    }
    rootItem=NULL;
    return;
}

void insertRetail(struct item* p){
    p->next=NULL;
    struct item* p0;
    p0 = rootRetail;
    if (p0 == NULL) {
        rootRetail = p;
        return;
    }
    else {
        while (p0->next != NULL)
            p0 = p0->next;
        p0->next = p;
        return;
    }
}

int searchRetail(struct item* p){
    struct item *t;
    t=rootRetail;
    int count=1;
    while(t!=NULL&&(strcmp(p->brand,t->brand)!=0|| strcmp(p->sonBrand, t->sonBrand) != 0||(p->volume!=t->volume)))
    {
        t=t->next;
        count++;
    }
    if(t==NULL)
    {
        count=0;
    }
    return count;
}

void deleteRetail(struct item* p,int n){
    struct item* t;
    if (p== rootRetail) {
        t = p;
        p=p->next;
        free(t);
        rootRetail = p;
    }
    else
    {
        t=rootRetail;
        for(int i=0;i<n-2;i++)
        {
            t=t->next;
        }
        p = p->next;
        free(t->next);
        t->next = p;
    }
    return;
}
