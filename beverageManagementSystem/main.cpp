#include "widget.h"
#include"login.h"
#include"logon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    loginDatabase();

    login L;
    L.setWindowTitle(QString("登录"));
    L.show();

    return a.exec();
}
