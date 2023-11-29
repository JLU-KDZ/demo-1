#ifndef LOGON_H
#define LOGON_H

#include <QDialog>

namespace Ui {
class Logon;
}

class Logon : public QDialog
{
    Q_OBJECT

public:
    explicit Logon(QWidget *parent = 0);
    ~Logon();

private slots:
    void on_pushButton_register_clicked();

    void on_pushButton_return_clicked();

    void on_pushButton_geticode_clicked();

private:
    Ui::Logon *ui;

private:
    void keyPressEvent(QKeyEvent *event);

};

#endif // LOGON_H
