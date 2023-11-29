#include "find.h"
#include "ui_find.h"
#include"login.h"
#include"smtp.h"
#include"calculate.h"
#include<QMessageBox>
#include<QKeyEvent>
#include<QModelIndex>
#include<QtSql/QSqlQuery>
#include<Qtsql/QSqlQueryModel>

long long codeFind=-1;//用于保存生成的验证码

Find::Find(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Find)
{
    ui->setupUi(this);
    ui->lineEdit_user->setFocus();
}

Find::~Find()
{
    delete ui;
}

/* 鼠标左键点击获取验证码按钮，触发槽函数
 * 获取验证码*/
void Find::on_pushButton_geticode_clicked()
{
    QString mail=ui->lineEdit_mail->text();
    if(mail.length()==0)//判断邮箱长度
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱为空！"),QMessageBox::Ok);//弹出警告提示框
        /*清空所有输入栏，并将光标定位至用户名文本框*/
        ui->lineEdit_user->clear();
        ui->lineEdit_new->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_user->setFocus();
        return;
    }
    if(!mail.contains("@qq.com"))//判断邮箱格式
    {
        QMessageBox::warning(this,tr("注册提示"),tr("QQ邮箱格式错误！"),QMessageBox::Ok);
        ui->lineEdit_user->clear();
        ui->lineEdit_new->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_user->setFocus();
        return;
    }
    int r[6];
    srand(time(0));//随机数种子
    for (int i=0; i<6; i++)
    {
        r[i] = getRand(0,9);
    }//生成6位[0，9]区间内的随机整数序列作为验证码
    codeFind=r[0]*10*10*10*10*10+r[1]*10*10*10*10+r[2]*10*10*10+r[3]*10*10+r[4]*10+r[5];
    QString num0=QString::number(codeFind);
    QString num="您好！\n您的验证码为:"+num0+"，用于酒水管理系统找回密码。\n请勿泄露和转发，如非本人操作，请忽略此消息。";
    QByteArray byte=num.toUtf8();//类型转换
    Smtp smtp("3029104906@qq.com","nouhhxmvbhlqdgja"); //发送人的邮箱，smtp授权码
    smtp.SendData(mail.toUtf8(),"验证码",byte);//收件人的邮箱，主题，正文
    return;
}

/* 鼠标左键点击重置按钮，触发槽函数
 * 修改数据库中该用户名对应的密码*/
void Find::on_pushButton_change_clicked()
{
    QString userName=ui->lineEdit_user->text();
    QString passWord=ui->lineEdit_new->text();
    QString mail=ui->lineEdit_mail->text();
    if(userName.length()==0||passWord.length()==0)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("用户名或新密码为空！"),QMessageBox::Ok);
        ui->lineEdit_user->clear();
        ui->lineEdit_new->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_user->setFocus();
        return;
    }
    /*调用SQLite中的指令查找用户是否存在*/
    QString q=QString("select count(*) from userinfo where username = '"+userName+"' and mail = '"+mail+"'");
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(q);
    QModelIndex index1=model->index(0,0);
    if(index1.data()==0)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("用户或邮箱不存在！"),QMessageBox::Ok);
        ui->lineEdit_user->clear();
        ui->lineEdit_new->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_user->setFocus();
        return;
    }
    QString icode=ui->lineEdit_icode->text();
    if(icode.toLongLong()!=codeFind)
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("验证码错误！"),QMessageBox::Ok);
        ui->lineEdit_icode->clear();
        ui->lineEdit_icode->setFocus();
        return;
    }
    /*调用SQLite中的指令修改用户名对应的密码*/
    QString cmd=QString("update userinfo set password = '"+passWord+"' where username = '"+userName+"' and mail = '"+mail+"'");
    QSqlQuery query;
    if(query.exec(cmd))
    {
        QMessageBox::information(this,"找回密码提示","密码重置成功");
        ui->lineEdit_user->clear();
        ui->lineEdit_new->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_user->setFocus();
        return;
    }
    else
    {
        QMessageBox::warning(this,tr("找回密码提示"),tr("密码重置失败！"),QMessageBox::Ok);
        ui->lineEdit_user->clear();
        ui->lineEdit_new->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_user->setFocus();
        return;
    }
    delete model;
    return;
}

/* 键盘事件
 * 按下方向键向上或向下移动光标位置
 * 按下Esc建返回到上一个界面*/
void Find::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up)
    {
        if(ui->lineEdit_new->hasFocus())
        {
            ui->lineEdit_user->setFocus();
        }
        if(ui->lineEdit_mail->hasFocus())
        {
            ui->lineEdit_new->setFocus();
        }
        if(ui->lineEdit_icode->hasFocus())
        {
            ui->lineEdit_mail->setFocus();
        }
    }
    if(event->key()==Qt::Key_Down)
    {
        if(ui->lineEdit_mail->hasFocus())
        {
            ui->lineEdit_icode->setFocus();
        }
        if(ui->lineEdit_new->hasFocus())
        {
            ui->lineEdit_mail->setFocus();
        }
        if(ui->lineEdit_user->hasFocus())
        {
            ui->lineEdit_new->setFocus();
        }
    }
    if(event->key()==Qt::Key_Escape)
    {
        this->close();
        login* L=new login;
        L->setWindowTitle(QString("登录"));
        L->show();
    }
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，回到登录界面*/
void Find::on_pushButton_return_clicked()
{
    this->close();
    login* L=new login;
    L->setWindowTitle(QString("登录"));//设置标题
    L->show();
    return;
}
