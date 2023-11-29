#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_confirm_clicked();

    void on_logon_clicked();

    void on_pushButton_findP_clicked();

private:
    Ui::login *ui;

private:
    void keyPressEvent(QKeyEvent *event);

};

void loginDatabase(void);

#endif // LOGIN_H
