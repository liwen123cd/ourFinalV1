#include "insertdialog.h"
#include "ui_insertdialog.h"

InsertDialog::InsertDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertDialog)
{
    ui->setupUi(this);
}

InsertDialog::~InsertDialog()
{
    delete ui;
}

// 确认
void InsertDialog::on_buttonBox_accepted()
{
    QString stoName = ui->lineEdit_storage->text();
    QString proName = ui->lineEdit_product->text();
    QString amount = ui->lineEdit_amount->text();
    emit sendMsg(stoName, proName, amount);
    close();
}

// 取消
void InsertDialog::on_buttonBox_rejected()
{
    close();
}
