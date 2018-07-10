#include "sys_add_user.h"
#include "ui_sys_add_user.h"
#include "sys_sqlite.h"

sys_add_user::sys_add_user(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::sys_add_user)
{
    ui->setupUi(this);
}

sys_add_user::~sys_add_user()
{
    delete ui;
}

void sys_add_user::on_pushButton_clicked()
{
    QString add_user_name = ui->lineEdit->text();
    QString add_user_pwd = ui->lineEdit_2->text();
    QString add_user_email = ui->lineEdit_3->text();

    if(NULL == add_user_name)
        QMessageBox::warning(this, tr("提示"),
                             "请填写用户名！");
    if(NULL == add_user_pwd)
        QMessageBox::warning(this, tr("提示"),
                             "请填写密码！");
    if(NULL == add_user_email)
        QMessageBox::warning(this, tr("提示"),
                             "请填写邮箱！");
    if(NULL != add_user_name && NULL != add_user_pwd
            && NULL != add_user_email)
    {
        if(!userCheck(add_user_name, SYS_USER_NUMBER))
        {
            QMessageBox::warning(this, tr("提示"),
                                 "用户已存在，请更换用户名！");
        }
        else
        {
            QString add_sql = QString("insert into Sys_User(User_Name, "
                                      "User_Pwd, User_Email, User_Is_Admin)");
            add_sql += QString(" VALUES('%1','%2','%3','%4')")
                    .arg(add_user_name)
                    .arg(add_user_pwd)
                    .arg(add_user_email)
                    .arg(1);

            QSqlQuery query;
            query.exec(add_sql);

            QMessageBox::information(this, "添加成功",
                                     "您已成功添加新用户！");
            this->close();
        }
    }
}

void sys_add_user::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
}
