#ifndef CHANGEANDFIND_H
#define CHANGEANDFIND_H

#include <QDialog>

namespace Ui {
class changeAndFind;
}

class changeAndFind : public QDialog
{
    Q_OBJECT

public:
    explicit changeAndFind(QWidget *parent = 0);
    ~changeAndFind();

private:
    Ui::changeAndFind *ui;
};

#endif // CHANGEANDFIND_H
