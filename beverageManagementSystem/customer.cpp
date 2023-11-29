#include"customer.h"

pCustomer rootCustomer=NULL;//客户链表的头指针

int customerInit(void) {
    FILE* fp;
    if (fopen_s(&fp, "E:\\fpInitC.txt", "r") != 0) {
        return 1;//打开文件失败，返回1传递到前端处理，下同
    }
    pCustomer p;
    p = (pCustomer)malloc(sizeof(struct customer));
    if (p == NULL) {
        fclose(fp);
        return 2;//动态空间申请失败，返回2传递到前端处理
    }
    while ((fscanf(fp, "%s%d", p->name,&(p->level)) != EOF)) {
        insertCustomer(p);
        p = NULL;
        p = (pCustomer)malloc(sizeof(struct customer));
        if (p == NULL) {
            fclose(fp);
            return 2;//动态空间申请失败，返回2传递到前端处理
        }
    }
    free(p);
    fclose(fp);
    return 0;
}

int customerIn(void){
    FILE* fp;
     if(fopen_s(&fp,"E:\\fpInC.txt", "r") != 0) {
       return 1;
    }
    pCustomer p;
    p = (pCustomer)malloc(sizeof(struct customer));
    if (p == NULL) {
        fclose(fp);
        return 2;
    }
    while (fscanf(fp, "%s%d", p->name,&p->level) != EOF) {
        if (searchCustomer(p) == 0){
            insertCustomer(p);//若用户在链表中不存在，则将其加至链表尾
            p = NULL;
            p = (pCustomer)malloc(sizeof(struct customer));
            if (p == NULL) {
                fclose(fp);
                return 2;
            }
        }
        else {
                changeCustomer(p->name,p->level);//若用户在链表中存在，则修改该用户的等级
             }
    }
    free(p);
    fclose(fp);
    return 0;
}

void insertCustomer(pCustomer p) {
    p->next=NULL;
    pCustomer p0;
    p0 = rootCustomer;
    if (p0 == NULL) {
        rootCustomer = p;
    }
    else {
        while (p0->next != NULL)
            p0 = p0->next;
        p0->next = p;
    }
    return;
}

int searchCustomer(pCustomer p) {
    pCustomer t;
    t = rootCustomer;
    while (t != NULL && (strcmp(t->name, p->name) != 0))
    {
        t = t->next;
    }
    if (t == NULL) {
        return 0;//若链表中不存在该客户，则认为客户等级为零
    }
    else
        return t->level;
}

void changeCustomer(char* customerName,int customerLevel){
    pCustomer t;
    t=rootCustomer;
    while (t != NULL && (strcmp(t->name, customerName) != 0))
    {
        t = t->next;
    }
    t->level=customerLevel;
    return;
}

void customerBubbleSort(pCustomer head){
    struct customer *cur,*tail;
    cur=head;
    tail=NULL;
    if(cur==NULL||cur->next==NULL){
        return;
    }
    while(cur!=tail){
        while(cur->next!=tail){
            if(cur->level<cur->next->level){
                int temp=cur->level;
                cur->level=cur->next->level;
                cur->next->level=temp;
                char temp1[30];
                strcpy(temp1,cur->name);
                strcpy(cur->name,cur->next->name);
                strcpy(cur->next->name,temp1);
            }
            cur=cur->next;
        }
        tail=cur;
        cur=head;
    }
    return;
}

void removeCustomer(pCustomer head){
    pCustomer temp;
    while(head!=NULL){
        temp=head;
        free(temp);
        head=head->next;
    }
    rootCustomer=NULL;
    return;
}
