#include "register.h"
#include "ui_register.h"
#include "sys_sqlite.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
}

Register::~Register()
{
    delete ui;
}

void Register::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    Login *login = new Login;
    login->show();
}

void Register::on_pushButton_clicked()
{
    if(textCheck())
    {
        QString reg_name = ui->lineEdit->text();
        QString reg_pwd = ui->lineEdit_2->text();
        QString reg_emial = ui->lineEdit_4->text();
        int admin;
        if(ui->comboBox->currentText() == "管理员")
            admin = 1;
        else
            admin = 0;

        if(userCheck(reg_name, SYS_USER_NUMBER))
        {

            QString add_sql = QString("insert into Sys_User(User_Name, "
                                      "User_Pwd, User_Email, User_Is_Admin)");
            add_sql += QString(" VALUES('%1','%2','%3','%4')")
                    .arg(reg_name)
                    .arg(reg_pwd)
                    .arg(reg_emial)
                    .arg(admin);

            QSqlQuery query;
            query.exec(add_sql);

            QMessageBox::information(this, "注册成功",
                                     "您已成功注册新用户，请返回去登陆！");
            this->close();
        }
        else
        {
            QMessageBox::warning(this, "注册失败",
                                 "该用户已经存在，请更换用户名重试！");
        }
    }
}

void Register::on_pushButton_2_clicked()
{
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

/*
 *
 * 增强程序的鲁棒性
 *
 * */
bool Register::textCheck()
{
    if(NULL == ui->lineEdit->text())
        QMessageBox::warning(this, "警告",
                             "用户名为空，请输入用户名！");
    if(NULL == ui->lineEdit_2->text())
        QMessageBox::warning(this, "警告",
                             "密码为空，请输入密码！");
    if(NULL == ui->lineEdit_4->text())
        QMessageBox::warning(this, "警告",
                             "邮箱为空，请输入邮箱地址！");
    if(ui->lineEdit_2->text() != ui->lineEdit_3->text())
    {
        ui->lineEdit_3->clear();
        QMessageBox::warning(this, "警告",
                             "两次密码输入不一致，请重新输入！");
    }
    if(ui->lineEdit->text() != NULL
            && ui->lineEdit_2->text() != NULL
            && ui->lineEdit_3->text() != NULL
            && ui->lineEdit_4->text() != NULL
            && ui->lineEdit_2->text() == ui->lineEdit_3->text())
        return true;
    return false;
}
