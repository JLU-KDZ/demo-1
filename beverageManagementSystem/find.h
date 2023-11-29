#ifndef FIND_H
#define FIND_H

#include <QDialog>

namespace Ui {
class Find;
}

class Find : public QDialog
{
    Q_OBJECT

public:
    explicit Find(QWidget *parent = 0);
    ~Find();

private slots:
    void on_pushButton_geticode_clicked();

    void on_pushButton_return_clicked();

    void on_pushButton_change_clicked();

private:
    Ui::Find *ui;

private:

    void keyPressEvent(QKeyEvent *event);

};

#endif // FIND_H
