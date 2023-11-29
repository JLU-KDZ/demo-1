#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_check_clicked();

    void on_pushButton_init_clicked();

    void on_pushButton_returnItem_clicked();

    void on_pushButton_item_clicked();

    void on_pushButton_in_clicked();

    void on_pushButton_return_r_1_clicked();

    void on_pushButton_return_login_clicked();

    void on_pushButton_customer_clicked();

    void on_pushButton_return_1_clicked();

    void on_pushButton_remove_clicked();

    void on_pushButton_return_2_clicked();

    void on_pushButton_return_3_clicked();

    void on_pushButton_customer_init_clicked();

    void on_pushButton_returnCustomer_clicked();

    void on_pushButton_customer_check_clicked();

    void on_pushButton_c_remove_clicked();

    void on_pushButton_customer_in_clicked();

    void on_pushButton_r_in_clicked();

    void on_pushButton_r_out_clicked();

    void on_pushButton_return_r_2_clicked();

    void on_pushButton_records_clicked();

    void on_pushButton_out_clicked();

    void on_pushButton_c_in_clicked();

    void on_pushButton_return_r_3_clicked();

    void on_pushButton_c_out_clicked();

    void on_pushButton_return_r_4_clicked();

    void on_pushButton_clearI_clicked();

    void on_pushButton_writeI_clicked();

    void on_pushButton_clearO_clicked();

    void on_pushButton_writeO_clicked();

    void on_pushButton_writeInB_clicked();

    void on_pushButton_writeOutB_clicked();

    void on_pushButton_retail_clicked();

    void on_pushButton_buy_clicked();

    void on_pushButton_returnItem2_clicked();

    void on_pushButton_refresh_clicked();

private:
    Ui::Widget *ui;

private:
    void keyPressEvent(QKeyEvent *event);

};

#endif // WIDGET_H
