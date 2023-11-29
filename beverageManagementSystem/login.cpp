 #include "login.h"
#include "ui_login.h"
#include"widget.h"
#include "logon.h"
#include"find.h"
#include<QString>
/*Qt中调试程序使用的头文件*/
#include<QDebug>
#include<QMessageBox>
/*Qt中用于键盘事件的头文件*/
#include<QKeyEvent>
/*Qt中操作SQLite数据库的头文件*/
#include<QtSql/QSql>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlTableModel>
#include<QtSql/QSqlDatabase>
login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

/* 鼠标左键点击登录按钮，触发槽函数
 * 将输入的用户名和密码与数据库中的数据进行比对，判定登陆成功或失败*/
void login::on_confirm_clicked()
{
    QString userName=ui->username_->text();
    QString passWord=ui->password_->text();
    if(userName.length()==0||passWord.length()==0)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码为空！"),QMessageBox::Ok);
        ui->username_->clear();
        ui->password_->clear();
        ui->username_->setFocus();
        return;
    }
    /*在数据库中查询用户名和密码*/
    QSqlTableModel *model=new QSqlTableModel;
    model->setTable("userinfo");
    model->setFilter(QString("username='%1' and password='%2'").arg(userName).arg(passWord));
    model->select();
    int row=model->rowCount();
    if((ui->username_->text() == "admin" && ui->password_->text() == "123456")||row>0)//默认用户名：admin，默认密码：123456
    {
           this->close();
           Widget* W=new Widget;
           W->show();
           W->setWindowTitle(QString("酒水管理系统"));//登陆成功，关闭当前界面，打开主界面
       }else{
           QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码错误！"),QMessageBox::Ok);
           ui->username_->clear();
           ui->password_->clear();
           ui->username_->setFocus();
       }
    delete model;
    return;
}

/* 鼠标左键点击注册按钮，触发槽函数
 * 关闭当前界面，打开注册界面*/
void login::on_logon_clicked()
{
    this->close();
    Logon* R=new Logon;
    R->setWindowTitle(QString("注册"));
    R->show();
    return;
}

/* 鼠标左键点击找回密码按钮，触发槽函数
 * 关闭当前界面，打开找回密码界面*/
void login::on_pushButton_findP_clicked()
{
    this->close();
    Find* F=new Find;
    F->setWindowTitle(QString("找回密码"));
    F->show();
    return;
}

/* 用户数据库的建立
 * 在控制台输出数据库的打开和运行状况*/
void loginDatabase(void){
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("pwd.db");//数据库名为：pwd.db
    db.open();//打开
    if(db.open())
    {
       qDebug()<<"open database suc";

    }else{
       qDebug()<<"open database fail";
    }
    QString cmd="create table if not exists userinfo(username varchar(64),password varchar(64),mail varchar(64))";
    //数据库表的三个字段为：username，password，mail
    QSqlQuery query;
    if (query.exec(cmd)){//运行
        qDebug()<<"exec suc";
    }
    else{
        qDebug()<<"exec fail";
    }
    return;
}

/* 键盘事件
 * 按下方向键向上或向下移动光标位置
 * 按下Esc建返回到上一个界面*/
void login::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Up)
    {
        ui->username_->setFocus();
    }
    if(event->key()==Qt::Key_Down)
    {
        ui->password_->setFocus();
    }
    if(event->key()==Qt::Key_Escape)
    {
        this->close();
        exit(0);//在登陆界面按下Esc，关闭整个程序
    }
    return;
}

