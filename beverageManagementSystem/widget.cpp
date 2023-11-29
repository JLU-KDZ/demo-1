#include "widget.h"
#include "ui_widget.h"
#include"item.h"
#include"customer.h"
#include"calculate.h"
#include"login.h"
#include<time.h>
#include<Qstring>
#include<QStringlist>
#include<QFile>
#include<QKeyEvent>
#include<QTextStream>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QMessageBox>
#include<QTextCodec>
#include<QHeaderView>

/* 商品数量<=N1，商品库存不足，标红
 * N1<商品数量<N2,商品库存正常，标黑
 * 商品数量>=N2，商品库存堆积，标绿，八折出售*/
#define N1 20
#define N2 200
/*酒水品牌的数量*/
#define N 10
/*CountIn,CountOut记录进货和出货次数*/
int CountIn=0;
int CountOut=0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

/* 鼠标左键点击商品按钮，触发槽函数
 * 关闭当前界面，打开商品界面*/
void Widget::on_pushButton_item_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_item);
    return;
}

/* 鼠标左键点击客户按钮，触发槽函数
 * 关闭当前界面，打开客户界面*/
void Widget::on_pushButton_customer_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_customer);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开历史记录界面*/
void Widget::on_pushButton_records_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_records);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开登录界面*/
void Widget::on_pushButton_return_login_clicked()
{
    this->close();
    login* L=new login;
    L->setWindowTitle(QString("登录"));
    L->show();
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开主界面*/
void Widget::on_pushButton_return_1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开主界面*/
void Widget::on_pushButton_return_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开主界面*/
void Widget::on_pushButton_return_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_menu);
    return;
}

/* 鼠标左键点击建立库存按钮，触发槽函数
 * 将文件中的内容导入表格*/
void Widget::on_pushButton_init_clicked()
{
    int n=itemInit();
    if(n==1)
    {
        QMessageBox::warning(this,tr("商品初始化"),tr("文件打开失败！"),QMessageBox::Ok);
    }
    if(n==2)
    {
        QMessageBox::warning(this,tr("商品初始化"),tr("动态内存分配失败！"),QMessageBox::Ok);
    }
    if(n==0)
    {
        QMessageBox::information(this,"商品初始化","商品初始化成功");
    }
    return;
}

/* 鼠标左键点击查看库存按钮，触发槽函数
 * 关闭当前界面，打开库存界面*/
void Widget::on_pushButton_check_clicked()
{
    int count;
    if(ui->tableWidget_Item->rowCount()==0)//设置表头
    {
        ui->tableWidget_Item->setColumnCount(6);
        ui->tableWidget_Item->setRowCount(1);
        QStringList a;
        a<<"品牌"<<"子品牌"<<"容量(毫升)"<<"规格(件/箱)"<<"价格(元)"<<"数量(箱)";
        ui->tableWidget_Item->setHorizontalHeaderLabels(a);
    }
    pItemNode p=rootItem;
    while(p!=NULL)//向表中添加数据
    {
        count=ui->tableWidget_Item->rowCount();
        ui->tableWidget_Item->insertRow(count);
        /*添加数据并进行类型转换*/
        ui->tableWidget_Item->setItem(count-1,0, new QTableWidgetItem(QString::fromStdString(p->item0.brand)));
        ui->tableWidget_Item->setItem(count-1,1, new QTableWidgetItem(QString::fromStdString(p->item0.sonBrand)));
        ui->tableWidget_Item->setItem(count-1,2, new QTableWidgetItem(QString::number(p->item0.volume)));
        ui->tableWidget_Item->setItem(count-1,3, new QTableWidgetItem(QString::number(p->item0.size)));
        ui->tableWidget_Item->setItem(count-1,4, new QTableWidgetItem(QString::number(p->item0.price)));
        ui->tableWidget_Item->setItem(count-1,5, new QTableWidgetItem(QString::number(p->amount)));
        if(p->amount<=N1)
        {
            ui->tableWidget_Item->item(count-1,5)->setForeground(Qt::red);
        }
        if(p->amount>N1&&p->amount<N2)
        {
            ui->tableWidget_Item->item(count-1,5)->setForeground(Qt::black);
        }
        if(p->amount>=N2)
        {
            ui->tableWidget_Item->item(count-1,5)->setForeground(Qt::green);
        }
        p=p->next;
    }
    ui->tableWidget_Item->removeRow(ui->tableWidget_Item->rowCount()-1);//删掉多余的一行
    ui->tableWidget_Item->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置表格自适应宽度
    ui->stackedWidget->setCurrentWidget(ui->page_item_1);//转到表格界面
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开商品界面*/
void Widget::on_pushButton_returnItem_clicked()
{
    /*清空表格并关闭界面*/
    int count;
    count=ui->tableWidget_Item->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_Item->removeRow(0);
    }
    ui->stackedWidget->setCurrentWidget(ui->page_item);
    return;
}

/* 鼠标左键点击清除库存按钮，触发槽函数
 * 清空库存表内的所有内容*/
void Widget::on_pushButton_remove_clicked()
{
    /*清空表格并清空商品链表*/
    removeItem(rootItem);
    int count;
    count=ui->tableWidget_Item->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_Item->removeRow(0);
    }
    return;
}

/* 鼠标左键点击零售按钮，触发槽函数
 * 关闭当前界面，打开零售界面*/
void Widget::on_pushButton_retail_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_retail);
}

/* 鼠标左键点击购买按钮，触发槽函数
 * 进行零售购买*/
void Widget::on_pushButton_buy_clicked()
{
    ui->lineEdit_price->clear();
    const char* b;
    const char* sb;
    int v;
    if(ui->lineEdit_brand->text().length()==0)
    {
        QMessageBox::warning(this,tr("零售"),tr("品牌为空！"),QMessageBox::Ok);
        return;
    }else
    {
        b=ui->lineEdit_brand->text().toStdString().c_str();
    }
    if(ui->lineEdit_sonBrand->text().length()==0)
    {
        QMessageBox::warning(this,tr("零售"),tr("子品牌为空！"),QMessageBox::Ok);
        return;
    }else
    {
        sb=ui->lineEdit_sonBrand->text().toStdString().c_str();
    }
    if(ui->lineEdit_volume->text().length()==0)
    {
        QMessageBox::warning(this,tr("零售"),tr("容量为空！"),QMessageBox::Ok);
        return;
    }else
    {
        v=ui->lineEdit_volume->text().toInt();
    }
    struct item* p;
    p=(struct item*)malloc(sizeof(struct item));
    strcpy(p->brand,b);
    strcpy(p->sonBrand,sb);
    p->volume=v;//用于保存需求商品的结点
    int n=searchRetail(p);
    if(n!=0)//若零售商品链表中存在该商品，则优先出售零售链表中的商品
    {
        struct item* t;
        t=rootRetail;
        for(int i=0;i<n-1;i++)
        {
            t=t->next;
        }
        ui->lineEdit_price->setText(QString::number(t->price));
        (t->size)--;
        if(t->size==0)
        {
            deleteRetail(t,n);
        }
    }
    else//若零售商品链表中不存在该商品，则在总商品链表中搜索
    {
        pItemNode pitem;
        pitem=(pItemNode)malloc(sizeof(pItemNode));
        strcpy(pitem->item0.brand,p->brand);
        strcpy(pitem->item0.sonBrand,p->sonBrand);
        pitem->item0.volume=p->volume;
        flagRetail=0;
        pItemNode ptr=searchItem(pitem);
        flagRetail=1;
        if(ptr!=NULL)//总商品链表中若存在，则将商品的一箱拆开作零售
        {
            ptr->amount--;
            if(ptr->amount==0)
            {
                deleteItem(ptr);
            }
            ui->lineEdit_price->setText(QString::number(ptr->item0.price));
            p->price=ptr->item0.price;
            p->size=ptr->item0.size-1;
            if(p->size!=0)
                insertRetail(p);
        }
        else//总商品链表中若不存在，则提示库存不足
        {
            QMessageBox::warning(this,tr("零售"),tr("库存不足！"),QMessageBox::Ok);
        }
        tempPtr0=NULL;
        tempPtr=rootItem;
    }
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 将更新后商品链表重写进表格*/
void Widget::on_pushButton_refresh_clicked()
{
    int c;
    c=ui->tableWidget_retail->rowCount();
    for(int i=0;i<c;i++)
    {
        ui->tableWidget_retail->removeRow(0);
    }
    int count;
    if(ui->tableWidget_retail->rowCount()==0)//设置表头
    {
        ui->tableWidget_retail->setColumnCount(5);
        ui->tableWidget_retail->setRowCount(1);
        QStringList a;
        a<<"品牌"<<"子品牌"<<"容量(毫升)"<<"零售数量(件)"<<"零售价格(元)";
        ui->tableWidget_retail->setHorizontalHeaderLabels(a);
    }
    struct item* p=rootRetail;
    while(p!=NULL)//向表中添加数据
    {
        count=ui->tableWidget_retail->rowCount();
        ui->tableWidget_retail->insertRow(count);
        /*添加数据并进行类型转换*/
        ui->tableWidget_retail->setItem(count-1,0, new QTableWidgetItem(QString::fromStdString(p->brand)));
        ui->tableWidget_retail->setItem(count-1,1, new QTableWidgetItem(QString::fromStdString(p->sonBrand)));
        ui->tableWidget_retail->setItem(count-1,2, new QTableWidgetItem(QString::number(p->volume)));
        ui->tableWidget_retail->setItem(count-1,3, new QTableWidgetItem(QString::number(p->size)));
        ui->tableWidget_retail->setItem(count-1,4, new QTableWidgetItem(QString::number(p->price)));
        p=p->next;
    }
    ui->tableWidget_retail->removeRow(ui->tableWidget_retail->rowCount()-1);//删掉多余的一行
    ui->tableWidget_retail->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置表格自适应宽度
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开商品界面*/
void Widget::on_pushButton_returnItem2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_item);
}

/* 鼠标左键点击进货按钮，触发槽函数
 * 将文件中的内容写入进货记录表格，并计算出进货成本*/
void Widget::on_pushButton_in_clicked()
{   ui->lineEdit_cost->clear();
    int n=itemIn();
    if(n==1)
    {
        QMessageBox::warning(this,tr("进货"),tr("文件打开失败！"),QMessageBox::Ok);
    }
    if(n==2)
    {
        QMessageBox::warning(this,tr("进货"),tr("动态内存分配失败！"),QMessageBox::Ok);
    }
    if(n==0)
    {
        CountIn++;
        QMessageBox::information(this,"进货","进货成功\n请进入进货记录查看");
    }
    if(n==0)
    {
    if(ui->tableWidget_r_i->rowCount()==0){
        ui->tableWidget_r_i->setColumnCount(6);
        ui->tableWidget_r_i->setRowCount(1);
        QStringList a;
        a<<"品牌"<<"子品牌"<<"容量(毫升)"<<"规格(件/箱)"<<"价格(元)"<<"数量(箱)";
        ui->tableWidget_r_i->setHorizontalHeaderLabels(a);
    }
    /*获取本地时间*/
    time_t now=time(0);
    char dt[50];
    ctime_s(dt,sizeof(dt),&now);
    long long Cost;
    Cost=cost();//计算成本
    if(Cost==-1)
    {
        QMessageBox::warning(this,tr("进货"),tr("文件打开失败！"),QMessageBox::Ok);
    }
    if(Cost==-2)
    {
        QMessageBox::warning(this,tr("进货"),tr("动态内存分配失败！"),QMessageBox::Ok);
    }
    if(ui->tableWidget_r_i->rowCount()!=1)
    {
        ui->tableWidget_r_i->insertRow(ui->tableWidget_r_i->rowCount());
    }
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,0, new QTableWidgetItem(QString("--")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,1, new QTableWidgetItem(QString("数量")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,2, new QTableWidgetItem(QString("时间")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,3, new QTableWidgetItem(QString("成本")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,4, new QTableWidgetItem(QString("--")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,5, new QTableWidgetItem(QString("--")));
    ui->tableWidget_r_i->insertRow(ui->tableWidget_r_i->rowCount());
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,0, new QTableWidgetItem(QString("---")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,1, new QTableWidgetItem(QString::number(CountIn)));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,2, new QTableWidgetItem(QString::fromStdString(dt)));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,3, new QTableWidgetItem(QString::number(Cost)));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,4, new QTableWidgetItem(QString("---")));
    ui->tableWidget_r_i->setItem(ui->tableWidget_r_i->rowCount()-1,5, new QTableWidgetItem(QString("---")));
    ui->tableWidget_r_i->insertRow(ui->tableWidget_r_i->rowCount());
    ui->lineEdit_cost->setText(QString::number(Cost));//输出数据
    /*使用UTF-8编码模式从文件中直接读取并写入表格,防止出现中文乱码*/
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QFile fp("E:\\fpInI.txt");
    fp.open(QIODevice::ReadOnly);
    QTextStream in(&fp);//以文本流方式读取
    while(!in.atEnd()){
        int count;
        count=ui->tableWidget_r_i->rowCount();
        ui->tableWidget_r_i->insertRow(count);
        QString line=in.readLine();//读取一行数据
        if(line.length()==0)
        {
            break;
        }
        QStringList line0=line.split("\t",QString::SplitBehavior::SkipEmptyParts);//将读入的数据依据'\t'划分
        ui->tableWidget_r_i->setItem(count-1,0, new QTableWidgetItem(QString(line0[0])));
        ui->tableWidget_r_i->setItem(count-1,1, new QTableWidgetItem(QString(line0[1])));
        ui->tableWidget_r_i->setItem(count-1,2, new QTableWidgetItem(QString(line0[2])));
        ui->tableWidget_r_i->setItem(count-1,3, new QTableWidgetItem(QString(line0[3])));
        ui->tableWidget_r_i->setItem(count-1,4, new QTableWidgetItem(QString(line0[4])));
        ui->tableWidget_r_i->setItem(count-1,5, new QTableWidgetItem(QString(line0[5])));
    }
    ui->tableWidget_r_i->removeRow(ui->tableWidget_r_i->rowCount()-1);
    ui->tableWidget_r_i->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fp.close();
    return;
    }
}

/* 鼠标左键点击出货按钮，触发槽函数
 * 将文件中的内容写入出货记录表格，并计算出出货收入*/
void Widget::on_pushButton_out_clicked()
{   ui->lineEdit_revenue->clear();
    QString name=ui->lineEdit_c_name->text();
    if(name.length()==0)
    {
        QMessageBox::warning(this,tr("出货"),tr("客户名称为空！"),QMessageBox::Ok);
        return;
    }
    int n=itemOut();
    if(n==1)
    {
        QMessageBox::warning(this,tr("出货"),tr("文件打开失败！"),QMessageBox::Ok);
        return;
    }
    if(n==2)
    {
        QMessageBox::warning(this,tr("出货"),tr("动态内存分配失败！"),QMessageBox::Ok);
        return;
    }
    if(n==3)
    {
        QMessageBox::warning(this,tr("出货"),tr("商品库存不足！"),QMessageBox::Ok);
        return;
    }
    if(n==0)
    {
        CountOut++;
        QMessageBox::information(this,"出货","出货成功\n请进入出货记录查看");
    }
    if(n==0)
    {
    if(ui->tableWidget_r_o->rowCount()==0){
        ui->tableWidget_r_o->setColumnCount(6);
        ui->tableWidget_r_o->setRowCount(1);
        QStringList a;
        a<<"品牌"<<"子品牌"<<"容量(毫升)"<<"规格(件/箱)"<<"价格(元)"<<"数量(箱)";
        ui->tableWidget_r_o->setHorizontalHeaderLabels(a);
    }
    time_t now=time(0);
    char dt[50];
    ctime_s(dt,sizeof(dt),&now);
    long long Revenue;
    Revenue=revenue(name.toStdString().c_str());//计算收入
    if(Revenue==-1)
    {
        QMessageBox::warning(this,tr("出货"),tr("文件打开失败！"),QMessageBox::Ok);
    }
    if(Revenue==-2)
    {
        QMessageBox::warning(this,tr("出货"),tr("动态内存分配失败！"),QMessageBox::Ok);
    }
    if(ui->tableWidget_r_o->rowCount()!=1)
    {
        ui->tableWidget_r_o->insertRow(ui->tableWidget_r_o->rowCount());
    }
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,0, new QTableWidgetItem(QString("--")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,1, new QTableWidgetItem(QString("数量")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,2, new QTableWidgetItem(QString("时间")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,3, new QTableWidgetItem(QString("收入")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,4, new QTableWidgetItem(QString("--")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,5, new QTableWidgetItem(QString("--")));
    ui->tableWidget_r_o->insertRow(ui->tableWidget_r_o->rowCount());
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,0, new QTableWidgetItem(QString("---")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,1, new QTableWidgetItem(QString::number(CountOut)));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,2, new QTableWidgetItem(QString::fromStdString(dt)));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,3, new QTableWidgetItem(QString::number(Revenue)));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,4, new QTableWidgetItem(QString("---")));
    ui->tableWidget_r_o->setItem(ui->tableWidget_r_o->rowCount()-1,5, new QTableWidgetItem(QString("---")));
    ui->tableWidget_r_o->insertRow(ui->tableWidget_r_o->rowCount());
    ui->lineEdit_revenue->setText(QString::number(Revenue));
    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QFile fp("E:\\fpOutI.txt");
    fp.open(QIODevice::ReadOnly);
    QTextStream in(&fp);
    while(!in.atEnd()){
        int count;
        count=ui->tableWidget_r_o->rowCount();
        ui->tableWidget_r_o->insertRow(count);
        QString line=in.readLine();
        if(line.length()==0)
        {
            break;
        }
        QStringList line0=line.split("\t",QString::SplitBehavior::SkipEmptyParts);
        ui->tableWidget_r_o->setItem(count-1,0, new QTableWidgetItem(QString(line0[0])));
        ui->tableWidget_r_o->setItem(count-1,1, new QTableWidgetItem(QString(line0[1])));
        ui->tableWidget_r_o->setItem(count-1,2, new QTableWidgetItem(QString(line0[2])));
        ui->tableWidget_r_o->setItem(count-1,3, new QTableWidgetItem(QString(line0[3])));
        ui->tableWidget_r_o->setItem(count-1,4, new QTableWidgetItem(QString(line0[4])));
        ui->tableWidget_r_o->setItem(count-1,5, new QTableWidgetItem(QString(line0[5])));
    }
    ui->tableWidget_r_o->removeRow(ui->tableWidget_r_o->rowCount()-1);
    ui->tableWidget_r_o->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    fp.close();
    return;
    }
}

/* 鼠标左键点击客户初始化按钮，触发槽函数
 * 将文件中的客户导入表格*/
void Widget::on_pushButton_customer_init_clicked()
{
    int n=customerInit();
    if(n==1)
    {
        QMessageBox::warning(this,tr("客户初始化"),tr("文件打开失败！"),QMessageBox::Ok);
    }
    if(n==2)
    {
        QMessageBox::warning(this,tr("客户初始化"),tr("动态内存分配失败！"),QMessageBox::Ok);
    }
    if(n==0)
    {
        QMessageBox::information(this,"客户初始化","客户初始化成功");
    }
    return;
}

/* 鼠标左键点击查看客户按钮，触发槽函数
 * 关闭当前界面，打开客户查询界面*/
void Widget::on_pushButton_customer_check_clicked()
{
    customerBubbleSort(rootCustomer);//对客户依据等级排序
    int count;
    if(ui->tableWidget_c->rowCount()==0)
    {
        ui->tableWidget_c->setColumnCount(2);
        ui->tableWidget_c->setRowCount(1);
        QStringList a;
        a<<"客户名称"<<"客户等级";
        ui->tableWidget_c->setHorizontalHeaderLabels(a);
    }
    pCustomer p=rootCustomer;
    while(p!=NULL)
    {
        count=ui->tableWidget_c->rowCount();
        ui->tableWidget_c->insertRow(count);
        ui->tableWidget_c->setItem(count-1,0, new QTableWidgetItem(QString::fromStdString(p->name)));
        ui->tableWidget_c->setItem(count-1,1, new QTableWidgetItem(QString::number(p->level)));
        p=p->next;
    }
    ui->tableWidget_c->removeRow(ui->tableWidget_c->rowCount()-1);
    ui->tableWidget_c->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget(ui->page_customer_1);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开客户界面*/
void Widget::on_pushButton_returnCustomer_clicked()
{
    int count;
    count=ui->tableWidget_c->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_c->removeRow(0);
    }
    ui->stackedWidget->setCurrentWidget(ui->page_customer);
    return;
}

/* 鼠标左键点击清除客户按钮，触发槽函数
 * 清空客户表内的所有内容并清空客户链表*/
void Widget::on_pushButton_c_remove_clicked()
{
    removeCustomer(rootCustomer);
    int count;
    count=ui->tableWidget_c->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_c->removeRow(0);
    }
    return;
}

/* 鼠标左键点击添加/修改客户按钮，触发槽函数
 * 将文件中的内容添加到客户表或对客户表中的内容修改*/
void Widget::on_pushButton_customer_in_clicked()
{
    int n=customerIn();
    if(n==1)
    {
        QMessageBox::warning(this,tr("客户添加"),tr("文件打开失败！"),QMessageBox::Ok);
    }
    if(n==2)
    {
        QMessageBox::warning(this,tr("客户添加"),tr("动态内存分配失败！"),QMessageBox::Ok);
    }
    if(n==0)
    {
        QMessageBox::information(this,"客户添加","客户添加成功");
    }

    return;
}

/* 鼠标左键点击进货记录按钮，触发槽函数
 * 关闭当前界面，打开进货记录界面*/
void Widget::on_pushButton_r_in_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_r_i);
    return;
}

/* 鼠标左键点击清空记录按钮，触发槽函数
 * 清空表中的所有内容*/
void Widget::on_pushButton_clearI_clicked()
{
    int count;
    count=ui->tableWidget_r_i->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_r_i->removeRow(0);
    }
    return;
}

/* 鼠标左键点击写入本地文件按钮，触发槽函数
 * 将进货记录表格中的数据写入文件*/
void Widget::on_pushButton_writeI_clicked()
{
    int count;
    count=ui->tableWidget_r_i->rowCount();
    if(count<2)
    {
        QMessageBox::warning(this,tr("写入进货记录"),tr("进货记录不足！"),QMessageBox::Ok);
        return;
    }
    FILE* fp;
    if (fopen_s(&fp, "E:\\recordsIn.", "w") != 0) {
        QMessageBox::warning(this,tr("写入进货记录"),tr("文件打开失败！"),QMessageBox::Ok);
        return;
    }
    fprintf_s(fp,"%s\t\t","品牌");
    fprintf_s(fp,"%s\t\t","子品牌");
    fprintf_s(fp,"%s\t\t\t","容量");
    fprintf_s(fp,"%s\t\t\t","规格");
    fprintf_s(fp,"%s\t\t\t","单价");
    fprintf_s(fp,"%s\r\n","数量");
    for(int i=0;i<count;i++)
    {
        QString str=ui->tableWidget_r_i->item(i,0)->text();
        if(QString::compare(str,"--")==0)//写入记录的第一行内容
        {
            fprintf_s(fp,"%s\t\t","--");
            fprintf_s(fp,"%s\t\t","次数");
            fprintf_s(fp,"%s\t\t\t","时间");
            fprintf_s(fp,"%s\t\t\t","成本");
            fprintf_s(fp,"%s\t\t\t","--");
            fprintf_s(fp,"%s\r\n","--");
        }
        if(QString::compare(str,"---")==0)//写入记录的第二行内容
        {
            int num;
            const char* time;
            char time0[20]={0};
            long long cost;
            num=ui->tableWidget_r_i->item(i,1)->text().toInt();
            time=ui->tableWidget_r_i->item(i,2)->text().toStdString().c_str();
            cost=ui->tableWidget_r_i->item(i,3)->text().toLongLong();
            const char *p=time;
            while((*p)!=':')
            {
                p++;
            }
             p=p-2;
            for(int i=0;i<8;i++)
            {
                time0[i]=*p;
                p++;
            }
            time0[8]='\0';//此处仅显示进货对应的时间(24小时)
            fprintf_s(fp,"%s\t\t","---");
            fprintf_s(fp,"%d\t\t",num);
            fprintf_s(fp,"%s\t\t",time0);
            fprintf_s(fp,"%lld",cost);
            writeT(fp,cost);
            fprintf_s(fp,"%s","---");
            writeT(fp,"---");
            fprintf_s(fp,"%s\r\n","---");
        }
        if(QString::compare(str,"--")!=0&&QString::compare(str,"---")!=0)//写入商品信息
        {
            const char* brand;
            const char* sonBrand;
            int volume,size;
            long long price,amount;
            brand=ui->tableWidget_r_i->item(i,0)->text().toStdString().c_str();
            sonBrand=ui->tableWidget_r_i->item(i,1)->text().toStdString().c_str();
            volume=ui->tableWidget_r_i->item(i,2)->text().toInt();
            size=ui->tableWidget_r_i->item(i,3)->text().toInt();
            price=ui->tableWidget_r_i->item(i,4)->text().toLongLong();
            amount=ui->tableWidget_r_i->item(i,5)->text().toLongLong();
            fprintf_s(fp,"%s",brand);
            writeT(fp,brand);
            fprintf_s(fp,"%s",sonBrand);
            writeT(fp,sonBrand);
            fprintf_s(fp,"%d",volume);
            writeT(fp,volume);
            fprintf_s(fp,"%d",size);
            writeT(fp,size);
            fprintf_s(fp,"%lld",price);
            writeT(fp,price);
            fprintf_s(fp,"%lld\r\n",amount);
        }
    }
    QMessageBox::information(this,"写入文件提示","写入成功");
    fclose(fp);
    return;
}

/* 鼠标左键点击写入本地文件（二进制），触发槽函数
 * 将进货记录以数据块方式写入文件*/
void Widget::on_pushButton_writeInB_clicked()
{
    int count;
    count=ui->tableWidget_r_i->rowCount();
    if(count<2)
    {
        QMessageBox::warning(this,tr("写入进货记录"),tr("进货记录不足！"),QMessageBox::Ok);
        return;
    }
    FILE* fp;
    if (fopen_s(&fp, "E:\\recordsInB.dat", "wb") != 0) {//打开二进制文件
        QMessageBox::warning(this,tr("写入进货记录"),tr("文件打开失败！"),QMessageBox::Ok);
        return;
    }
    struct head
    {
        const char* Brand="品牌";
        const char* SonBrand="子品牌";
        const char* Volume="容量";
        const char* Size="规格";
        const char* Price="单价";
        const char* Amount="数量";
    } head0;
    fwrite(&head0,sizeof(struct head),1,fp);//写入表头
    for(int i=0;i<count;i++)
    {
        QString str=ui->tableWidget_r_i->item(i,0)->text();
        if(QString::compare(str,"--")==0)
        {
            struct record
            {
                const char* i0="--";
                const char* i1="次数";
                const char* i2="时间";
                const char* i3="成本";
                const char* i4="--";
                const char* i5="--";
            } record0;
            fwrite(&record0,sizeof(struct record),1,fp);
        }
        if(QString::compare(str,"---")==0)
        {   struct recordLine
            {
                const char* i0="---";
                int num;
                char time0[20]={0};
                long long cost;
                const char* i4="---";
                const char* i5="---";
            }recordLine0;
            const char* time;
            recordLine0.num=ui->tableWidget_r_i->item(i,1)->text().toInt();
            time=ui->tableWidget_r_i->item(i,2)->text().toStdString().c_str();
            recordLine0.cost=ui->tableWidget_r_i->item(i,3)->text().toLongLong();
            const char *p=time;
            while((*p)!=':')
            {
                p++;
            }
             p=p-2;
            for(int i=0;i<8;i++)
            {
                recordLine0.time0[i]=*p;
                p++;
            }
            recordLine0.time0[8]='\0';
            fwrite(&recordLine0,sizeof(struct recordLine),1,fp);
        }
        if(QString::compare(str,"--")!=0&&QString::compare(str,"---")!=0)
        {
            const char*p1,*p2;
            int j=0,k=0;
            pItemNode p;
            p=(pItemNode)malloc(sizeof(struct itemNode));
            p1=ui->tableWidget_r_i->item(i,0)->text().toStdString().c_str();
            p2=ui->tableWidget_r_i->item(i,1)->text().toStdString().c_str();
            p->item0.volume=ui->tableWidget_r_i->item(i,2)->text().toInt();
            p->item0.size=ui->tableWidget_r_i->item(i,3)->text().toInt();
            p->item0.price=ui->tableWidget_r_i->item(i,4)->text().toLongLong();
            p->amount=ui->tableWidget_r_i->item(i,5)->text().toLongLong();
            while(*(p1)!='\0')
            {
                p->item0.brand[j]=*(p1);
                j++;
                p1++;
            }
            while(*(p2)!='\0')
            {
                p->item0.sonBrand[k]=*(p2);
                k++;
                p2++;
            }
            p->item0.brand[j]='\0';
            p->item0.sonBrand[k]='\0';
            fwrite(p,sizeof(struct itemNode),1,fp);//使用商品结点写入文件
            free(p);
        }
    }
    QMessageBox::information(this,"写入文件提示","写入成功");
    fclose(fp);
    return;
}

/* 鼠标左键点击出货记录按钮，触发槽函数
 * 关闭当前界面，打开出货记录界面*/
void Widget::on_pushButton_r_out_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_r_o);
    return;
}

/* 鼠标左键点击清空记录按钮，触发槽函数
 * 清空表中的所有内容*/
void Widget::on_pushButton_clearO_clicked()
{
    int count;
    count=ui->tableWidget_r_o->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_r_o->removeRow(0);
    }
    return;
}

/* 鼠标左键点击写入本地文件按钮，触发槽函数
 * 将出货记录表格中的数据写入文件*/
void Widget::on_pushButton_writeO_clicked()
{
    int count;
    count=ui->tableWidget_r_o->rowCount();
    if(count<2)
    {
        QMessageBox::warning(this,tr("写入出货记录"),tr("出货记录不足！"),QMessageBox::Ok);
        return;
    }
    FILE* fp;
    if (fopen_s(&fp, "E:\\recordsOut.txt", "w") != 0) {
        QMessageBox::warning(this,tr("写入出货记录"),tr("文件打开失败！"),QMessageBox::Ok);
        return;
    }
    fprintf_s(fp,"%s\t\t","品牌");
    fprintf_s(fp,"%s\t\t","子品牌");
    fprintf_s(fp,"%s\t\t\t","容量");
    fprintf_s(fp,"%s\t\t\t","规格");
    fprintf_s(fp,"%s\t\t\t","单价");
    fprintf_s(fp,"%s\r\n","数量");
    for(int i=0;i<count;i++)
    {
        QString str=ui->tableWidget_r_o->item(i,0)->text();
        if(QString::compare(str,"--")==0)
        {
            fprintf_s(fp,"%s\t\t","--");
            fprintf_s(fp,"%s\t\t","次数");
            fprintf_s(fp,"%s\t\t\t","时间");
            fprintf_s(fp,"%s\t\t\t","收入");
            fprintf_s(fp,"%s\t\t\t","--");
            fprintf_s(fp,"%s\r\n","--");
        }
        if(QString::compare(str,"---")==0)
        {
            int num;
            const char* time;
            char time0[20]={0};
            long long revenue;
            num=ui->tableWidget_r_i->item(i,1)->text().toInt();
            time=ui->tableWidget_r_i->item(i,2)->text().toStdString().c_str();
            revenue=ui->tableWidget_r_i->item(i,3)->text().toLongLong();
            const char *p=time;
            while((*p)!=':')
            {
                p++;
            }
             p=p-2;
            for(int i=0;i<8;i++)
            {
                time0[i]=*p;
                p++;
            }
            time0[8]='\0';
            fprintf_s(fp,"%s\t\t","---");
            fprintf_s(fp,"%d\t\t",num);
            fprintf_s(fp,"%s\t\t",time0);
            fprintf_s(fp,"%lld",revenue);
            writeT(fp,revenue);
            fprintf_s(fp,"%s","---");
            writeT(fp,"---");
            fprintf_s(fp,"%s\r\n","---");
        }
        if(QString::compare(str,"--")!=0&&QString::compare(str,"---")!=0)
        {
            const char* brand;
            const char* sonBrand;
            int volume,size;
            long long price,amount;
            brand=ui->tableWidget_r_o->item(i,0)->text().toStdString().c_str();
            sonBrand=ui->tableWidget_r_o->item(i,1)->text().toStdString().c_str();
            volume=ui->tableWidget_r_o->item(i,2)->text().toInt();
            size=ui->tableWidget_r_o->item(i,3)->text().toInt();
            price=ui->tableWidget_r_o->item(i,4)->text().toLongLong();
            amount=ui->tableWidget_r_o->item(i,5)->text().toLongLong();
            fprintf_s(fp,"%s",brand);
            writeT(fp,brand);
            fprintf_s(fp,"%s",sonBrand);
            writeT(fp,sonBrand);
            fprintf_s(fp,"%d",volume);
            writeT(fp,volume);
            fprintf_s(fp,"%d",size);
            writeT(fp,size);
            fprintf_s(fp,"%lld",price);
            writeT(fp,price);
            fprintf_s(fp,"%lld\r\n",amount);
        }
    }
    QMessageBox::information(this,"写入文件提示","写入成功");
    fclose(fp);
    return;
}

/* 鼠标左键点击写入本地文件（二进制），触发槽函数
 * 将出货记录以数据块方式写入文件*/
void Widget::on_pushButton_writeOutB_clicked()
{
    int count;
    count=ui->tableWidget_r_o->rowCount();
    if(count<2)
    {
        QMessageBox::warning(this,tr("写入出货记录"),tr("出货记录不足！"),QMessageBox::Ok);
        return;
    }
    FILE* fp;
    if (fopen_s(&fp, "E:\\recordsOutB.dat", "wb") != 0) {
        QMessageBox::warning(this,tr("写入出货记录"),tr("文件打开失败！"),QMessageBox::Ok);
        return;
    }
    struct head
    {
        const char* Brand="品牌";
        const char* SonBrand="子品牌";
        const char* Volume="容量";
        const char* Size="规格";
        const char* Price="单价";
        const char* Amount="数量";
    } head0;
    fwrite(&head0,sizeof(struct head),1,fp);
    for(int i=0;i<count;i++)
    {
        QString str=ui->tableWidget_r_o->item(i,0)->text();
        if(QString::compare(str,"--")==0)
        {
            struct record
            {
                const char* i0="--";
                const char* i1="次数";
                const char* i2="时间";
                const char* i3="收入";
                const char* i4="--";
                const char* i5="--";
            } record0;
            fwrite(&record0,sizeof(struct record),1,fp);
        }
        if(QString::compare(str,"---")==0)
        {   struct recordLine
            {
                const char* i0="---";
                int num;
                char time0[20]={0};
                long long revenue;
                const char* i4="---";
                const char* i5="---";
            }recordLine0;
            const char* time;
            recordLine0.num=ui->tableWidget_r_i->item(i,1)->text().toInt();
            time=ui->tableWidget_r_i->item(i,2)->text().toStdString().c_str();
            recordLine0.revenue=ui->tableWidget_r_i->item(i,3)->text().toLongLong();
            const char *p=time;
            while((*p)!=':')
            {
                p++;
            }
             p=p-2;
            for(int i=0;i<8;i++)
            {
                recordLine0.time0[i]=*p;
                p++;
            }
            recordLine0.time0[8]='\0';
            fwrite(&recordLine0,sizeof(struct recordLine),1,fp);
        }
        if(QString::compare(str,"--")!=0&&QString::compare(str,"---")!=0)
        {
            const char*p1,*p2;
            int j=0,k=0;
            pItemNode p;
            p=(pItemNode)malloc(sizeof(struct itemNode));
            p1=ui->tableWidget_r_o->item(i,0)->text().toStdString().c_str();
            p2=ui->tableWidget_r_o->item(i,1)->text().toStdString().c_str();
            p->item0.volume=ui->tableWidget_r_o->item(i,2)->text().toInt();
            p->item0.size=ui->tableWidget_r_o->item(i,3)->text().toInt();
            p->item0.price=ui->tableWidget_r_o->item(i,4)->text().toLongLong();
            p->amount=ui->tableWidget_r_o->item(i,5)->text().toLongLong();
            while(*(p1)!='\0')
            {
                p->item0.brand[j]=*(p1);
                j++;
                p1++;
            }
            while(*(p2)!='\0')
            {
                p->item0.sonBrand[k]=*(p2);
                k++;
                p2++;
            }
            p->item0.brand[j]='\0';
            p->item0.sonBrand[k]='\0';
            fwrite(p,sizeof(struct itemNode),1,fp);
            free(p);
        }
    }
    QMessageBox::information(this,"写入文件提示","写入成功");
    fclose(fp);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开历史记录界面*/
void Widget::on_pushButton_return_r_1_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_records);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开历史记录界面*/
void Widget::on_pushButton_return_r_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_records);
    return;
}

/* 鼠标左键点击进货预测，触发槽函数
 * 从进货记录表中读取历史数据，计算出下次进货数据，写入进货预测表格*/
void Widget::on_pushButton_c_in_clicked()
{
    int count=ui->tableWidget_r_i->rowCount();
    int times=0;//进货次数
    long long brand[N]={0};//每次进货时每个品牌的数量
    long long result[N]={0};//每个品牌的预测结果
    const char* temp[N]={"统一","康师傅","农夫山泉","百事可乐","可口可乐","茅台","五粮液","剑南春","伏特加","拉菲"};
    for(int c=0;c<count;c++)
    {
        /*找到下一次记录的起始点*/
        QString str=ui->tableWidget_r_i->item(c,0)->text();
        if(QString::compare(str,"---")!=0&&QString::compare(str,"--")!=0)
        {
            int flag=-1;
            for(int j=0;j<N;j++)
            {
                if(QString::compare(str,QString(temp[j]))==0)
                flag=j;
            }
            /*每个品牌对应一个链表，分别计算*/
            switch(flag)
            {
            case 0:brand[0]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 1:brand[1]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 2:brand[2]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 3:brand[3]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 4:brand[4]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 5:brand[5]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 6:brand[6]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 7:brand[7]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 8:brand[8]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            case 9:brand[9]+=ui->tableWidget_r_i->item(c,5)->text().toLongLong();break;
            default:QMessageBox::warning(this,tr("进货预测"),tr("酒水品牌错误！"),QMessageBox::Ok);
                for(int i=0;i<N;i++)//清空链表
                {
                    removeNode(root[i]);
                    root[i]=NULL;
                }
                return;
            }
        }
        if(QString::compare(str,"---")==0||c==count-1)
        {
            if(times>0){
                node* p[N];
                for(int i=0;i<N;i++)
                {
                    p[i]=(node*)malloc(sizeof(node));
                    if(p[i]==NULL)
                    {
                        QMessageBox::warning(this,tr("进货预测"),tr("动态空间申请失败！"),QMessageBox::Ok);
                        for(int i=0;i<N;i++)
                        {
                            removeNode(root[i]);
                            root[i]=NULL;
                        }
                        return;
                    }
                    p[i]->amount=brand[i];
                    insertNode(p[i],i);//插入到对应的链表中
                }
            }
            if(c!=count-1)
           {
                times++;
            }
           for(int i=0;i<N;i++)
           {
               brand[i]=0;
           }//清零，用于读取下一组数据
        }
    }
    /*数据太少，无法预测*/
    if(times<=1)
    {
        for(int i=0;i<N;i++)
        {
            removeNode(root[i]);
            root[i]=NULL;
        }
        QMessageBox::warning(this,tr("进货预测"),tr("进货历史信息不足！"),QMessageBox::Ok);
        return;
    }
    for(int i=0;i<N;i++)
    {
        result[i]=(long long)back(root[i]);
        if(result[i]<0)
            result[i]=0;//进货商品至少为零
    }
    for(int i=0;i<N;i++)
    {
        removeNode(root[i]);
        root[i]=NULL;
    }
    /*写入表格*/
    if(ui->tableWidget_backi->rowCount()==0)
    {
        ui->tableWidget_backi->setColumnCount(2);
        ui->tableWidget_backi->setRowCount(1);
        QStringList a;
        a<<"品牌"<<"数量";
        ui->tableWidget_backi->setHorizontalHeaderLabels(a);
    }
    for(int i=0;i<N;i++)
    {
        ui->tableWidget_backi->setItem(ui->tableWidget_backi->rowCount()-1,0,new QTableWidgetItem(QString(temp[i])));
        ui->tableWidget_backi->setItem(ui->tableWidget_backi->rowCount()-1,1,new QTableWidgetItem(QString::number(result[i])));
        ui->tableWidget_backi->insertRow(ui->tableWidget_backi->rowCount());
    }
    ui->tableWidget_backi->removeRow(ui->tableWidget_backi->rowCount()-1);
    ui->tableWidget_backi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget(ui->page_backi);
    return;
}

/* 鼠标左键点击出货预测，触发槽函数
 * 从出货记录表中读取历史数据，计算出下次出货数据，写入出货预测表格*/
void Widget::on_pushButton_c_out_clicked()
{
    int count=ui->tableWidget_r_o->rowCount();
    int times=0;
    long long brand[N]={0};
    long long result[N]={0};
    const char* temp[N]={"统一","康师傅","农夫山泉","百事可乐","可口可乐","茅台","五粮液","剑南春","伏特加","拉菲"};
    for(int c=0;c<count;c++)
    {
        QString str=ui->tableWidget_r_o->item(c,0)->text();
        if(QString::compare(str,"---")!=0&&QString::compare(str,"--")!=0)
        {
            int flag=-1;
            for(int j=0;j<N;j++)
            {
                if(QString::compare(str,QString(temp[j]))==0)
                flag=j;
            }
            switch(flag)
            {
            case 0:brand[0]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 1:brand[1]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 2:brand[2]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 3:brand[3]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 4:brand[4]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 5:brand[5]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 6:brand[6]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 7:brand[7]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 8:brand[8]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            case 9:brand[9]+=ui->tableWidget_r_o->item(c,5)->text().toLongLong();break;
            default:QMessageBox::warning(this,tr("出货预测"),tr("酒水品牌错误！"),QMessageBox::Ok);
                for(int i=0;i<N;i++)//清空链表
                {
                    removeNode(root[i]);
                    root[i]=NULL;
                }
                return;
            }
        }
        if(QString::compare(str,"---")==0||c==count-1)
        {
            if(times>0){
                node* p[N];
                for(int i=0;i<N;i++)
                {
                    p[i]=(node*)malloc(sizeof(node));
                    if(p[i]==NULL)
                    {
                        QMessageBox::warning(this,tr("出货预测"),tr("动态空间申请失败！"),QMessageBox::Ok);
                        for(int i=0;i<N;i++)
                        {
                            removeNode(root[i]);
                            root[i]=NULL;
                        }
                        return;
                    }
                    p[i]->amount=brand[i];
                    insertNode(p[i],i);
                }
            }
            if(c!=count-1)
           {
                times++;
            }
           for(int i=0;i<N;i++)
           {
               brand[i]=0;
           }
        }
    }
    if(times<=1)
    {
        QMessageBox::warning(this,tr("出货预测"),tr("出货历史信息不足！"),QMessageBox::Ok);
        for(int i=0;i<N;i++)
        {
            removeNode(root[i]);
            root[i]=NULL;
        }
        return;
    }
    for(int i=0;i<N;i++)
    {
        result[i]=(long long)back(root[i]);
        if(result[i]<0)
            result[i]=0;
    }
    for(int i=0;i<N;i++)
    {
        removeNode(root[i]);
        root[i]=NULL;
    }
    if(ui->tableWidget_backo->rowCount()==0)
    {
        ui->tableWidget_backo->setColumnCount(2);
        ui->tableWidget_backo->setRowCount(1);
        QStringList a;
        a<<"品牌"<<"数量";
        ui->tableWidget_backo->setHorizontalHeaderLabels(a);
    }
    for(int i=0;i<N;i++)
    {
        ui->tableWidget_backo->setItem(ui->tableWidget_backo->rowCount()-1,0,new QTableWidgetItem(QString(temp[i])));
        ui->tableWidget_backo->setItem(ui->tableWidget_backo->rowCount()-1,1,new QTableWidgetItem(QString::number(result[i])));
        ui->tableWidget_backo->insertRow(ui->tableWidget_backo->rowCount());
    }
    ui->tableWidget_backo->removeRow(ui->tableWidget_backo->rowCount()-1);
    ui->tableWidget_backo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentWidget(ui->page_backo);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开历史记录界面
 * 清空链表和表格*/
void Widget::on_pushButton_return_r_3_clicked()
{
    for(int i=0;i<N;i++)
    {
        removeNode(root[i]);
    }
    int count;
    count=ui->tableWidget_backi->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_backi->removeRow(0);
    }
    ui->stackedWidget->setCurrentWidget(ui->page_records);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，打开历史记录界面*/
void Widget::on_pushButton_return_r_4_clicked()
{
    for(int i=0;i<10;i++)
    {
        removeNode(root[i]);
    }
    int count;
    count=ui->tableWidget_backo->rowCount();
    for(int i=0;i<count;i++)
    {
        ui->tableWidget_backo->removeRow(0);
    }
    ui->stackedWidget->setCurrentWidget(ui->page_records);
    return;
}

/* 键盘事件
 * 按下Esc建返回到登录界面*/
void Widget::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Escape)
    {
        this->close();
        login* L=new login;
        L->setWindowTitle(QString("登录"));
        L->show();
    }
    return;
}
