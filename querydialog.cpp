#include "querydialog.h"
#include "ui_querydialog.h"
#include <QString>

QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

// 确认
void QueryDialog::on_buttonBox_accepted()
{
    QString option = ui->combo_query_by->currentText();
    QString content = ui->lineEdit->text();
    emit sendMsg(option, content);
    close();
}

// 取消
void QueryDialog::on_buttonBox_rejected()
{
    close();
}
