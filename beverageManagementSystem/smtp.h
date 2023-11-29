#ifndef SMTP_H
#define SMTP_H

#include<QByteArray>
#include<QString>
/*Qt中关于TCP的头文件*/
#include<QTcpSocket>
#include<QTcpServer>
#include<QHostAddress>

class Smtp
{
public:
    Smtp(QByteArray username,QByteArray password);
    ~Smtp();
    void SendData(QByteArray sendIp,QByteArray s_Title,QByteArray s_Content);
    QString WaitAndReadData();

private:
    QByteArray m_UserName="";
    QByteArray m_Password="";
    QTcpSocket * m_pSocket=nullptr;
    QString m_ReceiverData="";

};

#endif // SMTP_H
