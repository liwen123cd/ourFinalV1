#include "login.h"
#include "ui_login.h"

#include "recovepwd.h"
#include "sys_sqlite.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

Login::~Login()
{
    delete ui;
}

void Login::reshow()
{
    this->show();
}

void Login::on_pushButton_clicked()
{
    QString login_name = ui->lineEdit->text();
    QString login_pwd = ui->lineEdit_2->text();
    QString admin = ui->comboBox->currentText();
    int identify;
    if(admin == "管理员")
        identify = 1;
    else
    {
        identify = 0;
        recordSeller(login_name);
    }
    if(NULL == login_name)
        QMessageBox::warning(this, "警告",
                             "用户名为空，请输入用户名！");
    if(NULL == login_pwd)
        QMessageBox::warning(this, "警告",
                             "密码为空，请输入密码！");

    QSqlTableModel model;
    model.setTable("Sys_User");
    model.setFilter(tr("User_Name = '%1' and User_Pwd = '%2' "
                       "and User_Is_Admin = '%3'")
                    .arg(login_name)
                    .arg(login_pwd)
                    .arg(identify));
    model.select();

    if(model.rowCount() == 1)
    {
        // 表示密码验证正确，登陆成功
        // ...... 显示登录后的状态
        QMessageBox::information(this, "登录", "恭喜您登录成功！");
        //留着跳转到主窗口

        Data::is_admin = identify;
        qDebug()<<Data::is_admin;

        // 如果是卖家登录，则取数据库查询卖家的信息
        // 保存在 User 信息中

        //         recordSeller(login_name);

        //        qDebug()<<User::id;
        //        qDebug()<<User::name;
        //        qDebug()<<User::phone;
        //        qDebug()<<User::addr;

        //        User::name = login_name;
        MainWindow *mainwindow = new MainWindow;
        mainwindow->show();
        this->hide();
    }
    else if(userCheck(login_name, SYS_USER_NUMBER))
    {
        QMessageBox::warning(this, "警告",
                             "用户名不存在，请去注册！");
    }
    else if(!identityCheck(login_name, identify))
    {
        QMessageBox::warning(this, "警告",
                             "身份错误，请确认身份！");
    }
    else if(!pwdCheck(login_name, login_pwd))
    {
        QMessageBox::warning(this, "警告",
                             "密码错误，请重新输入！");
    }
}

void Login::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Login::on_pushButton_3_clicked()
{
    Register *r = new Register;
    this->close();
    r->show();
}

void Login::on_pushButton_4_clicked()
{
    RecovePwd *rp = new RecovePwd;
    rp->show();
}
