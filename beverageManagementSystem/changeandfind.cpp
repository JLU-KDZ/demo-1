#include "changeandfind.h"
#include "ui_changeandfind.h"

changeAndFind::changeAndFind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeAndFind)
{
    ui->setupUi(this);
}

changeAndFind::~changeAndFind()
{
    delete ui;
}
