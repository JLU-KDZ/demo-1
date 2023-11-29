#include "logon.h"
#include "ui_logon.h"
#include "login.h"
#include"smtp.h"
#include"calculate.h"
#include<QString>
#include<QByteArray>
#include<QMessageBox>
#include<QKeyEvent>
#include<QtSql/QSql>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlTableModel>
#include<Qtsql/QSqlQueryModel>
#include<QtSql/QSqlDatabase>
#include<QModelIndex>

long long code=-1;//用于保存生成的验证码

Logon::Logon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logon)
{
    ui->setupUi(this);
    ui->lineEdit_username->setFocus();
}

Logon::~Logon()
{
    delete ui;
}

/* 鼠标左键点击注册按钮，触发槽函数
 * 在数据库中插入用户名和密码*/
void Logon::on_pushButton_register_clicked()
{
    QString userName=ui->lineEdit_username->text();
    QString passWord=ui->lineEdit_password->text();
    QString passWord2=ui->lineEdit_r_password->text();
    QString mail=ui->lineEdit_mail->text();
    if(userName.length()==0||passWord.length()==0||passWord2.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("用户名或密码为空！"),QMessageBox::Ok);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    if(QString::compare(passWord,passWord2)!=0)//将两次输入的密码比对
    {
        QMessageBox::warning(this,tr("注册提示"),tr("两次输入密码不一致！"),QMessageBox::Ok);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    /*调用SQLite中的指令搜索该用户*/
    QString q=QString("select count(*) from userinfo where username='"+userName+"'");
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(q);
    QModelIndex index1=model->index(0,0);
    if(index1.data()!=0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("该用户已存在！"),QMessageBox::Ok);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    QString icode=ui->lineEdit_icode->text();
    if(icode.toLongLong()!=code)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("验证码错误！"),QMessageBox::Ok);
        ui->lineEdit_icode->clear();
        ui->lineEdit_icode->setFocus();
        return;
    }
    /*调用SQLite中的指令插入该用户*/
    QString cmd=QString("insert into userinfo values('%1','%2','%3')").arg(userName).arg(passWord).arg(mail);
    QSqlQuery query;
    if(query.exec(cmd))
    {
        QMessageBox::information(this,"注册提示","注册成功");
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    else
    {
        QMessageBox::warning(this,tr("注册提示"),tr("注册失败！"),QMessageBox::Ok);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    delete model;
    return;
}

/* 鼠标左键点击获取验证码按钮，触发槽函数
 * 获取验证码*/
void Logon::on_pushButton_geticode_clicked()
{
    QString mail=ui->lineEdit_mail->text();
    if(mail.length()==0)
    {
        QMessageBox::warning(this,tr("注册提示"),tr("邮箱为空！"),QMessageBox::Ok);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    if(!mail.contains("@qq.com"))
    {
        QMessageBox::warning(this,tr("注册提示"),tr("QQ邮箱格式错误！"),QMessageBox::Ok);
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_r_password->clear();
        ui->lineEdit_mail->clear();
        ui->lineEdit_icode->clear();
        ui->lineEdit_username->setFocus();
        return;
    }
    int r[6];
    srand(time(0));
    for (int i=0; i<6; i++)
    {
        r[i] = getRand(0,9);
    }
    code=r[0]*10*10*10*10*10+r[1]*10*10*10*10+r[2]*10*10*10+r[3]*10*10+r[4]*10+r[5];
    QString num0=QString::number(code);
    QString num="您好！\n您的验证码为:"+num0+"，用于酒水管理系统用户注册。\n请勿泄露和转发，如非本人操作，请忽略此消息。";
    QByteArray byte=num.toUtf8();
    Smtp smtp("3029104906@qq.com","nouhhxmvbhlqdgja");
    smtp.SendData(mail.toUtf8(),"验证码",byte);
    return;
}

/* 鼠标左键点击返回按钮，触发槽函数
 * 关闭当前界面，回到登录界面*/
void Logon::on_pushButton_return_clicked()
{
    this->close();
    login* L=new login;
    L->setWindowTitle(QString("登录"));
    L->show();
    return;
}

/* 键盘事件
 * 按下方向键向上或向下移动光标位置
 * 按下Esc建返回到登录界面*/
void Logon::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up)
    {
        if(ui->lineEdit_password->hasFocus())
        {
            ui->lineEdit_username->setFocus();
        }
        if(ui->lineEdit_r_password->hasFocus())
        {
            ui->lineEdit_password->setFocus();
        }
        if(ui->lineEdit_mail->hasFocus())
        {
            ui->lineEdit_r_password->setFocus();
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
        if(ui->lineEdit_r_password->hasFocus())
        {
            ui->lineEdit_mail->setFocus();
        }
        if(ui->lineEdit_password->hasFocus())
        {
            ui->lineEdit_r_password->setFocus();
        }
        if(ui->lineEdit_username->hasFocus())
        {
            ui->lineEdit_password->setFocus();
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
