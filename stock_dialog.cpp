#include "stock_dialog.h"
#include "ui_stock_dialog.h"

stock_Dialog::stock_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::stock_Dialog)
{
    ui->setupUi(this);

}

stock_Dialog::~stock_Dialog()
{
    delete ui;
}

void stock_Dialog::set_content(QString product_name,float price,QString provider_name,QString address)
{
    ui->lineEdit->setText(product_name);
    ui->lineEdit_2->setText(tr("%1").arg(price));
    ui->lineEdit_3->setText(provider_name);
    ui->lineEdit_4->setText(address);
}
