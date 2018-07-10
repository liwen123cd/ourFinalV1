#include "usermanager.h"
#include "ui_usermanager.h"
#include "sys_sqlite.h"
#include "sys_add_user.h"

UserManager::UserManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);
//    tableModel();
    model = new QSqlTableModel(this);
    model->setTable("Sys_User");
    // 系统管理只能物流公司使用，即管理系统管理员
    model->setFilter("User_Is_Admin = 1");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("用户编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("用户名"));
    model->setHeaderData(2, Qt::Horizontal, tr("用户密码"));
    model->setHeaderData(3, Qt::Horizontal, tr("用户邮箱"));
    model->setHeaderData(4, Qt::Horizontal, tr("用户身份"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);
    // 不显示用户ID和身份，这些信息不必要
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(4);
    //    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

UserManager::~UserManager()
{
    delete ui;
}

void UserManager::on_pushButton_2_clicked()
{
    int curRow = ui->tableView->currentIndex().row();

    if(-1 == curRow)
    {
        QMessageBox::warning(this, tr("提示"),
                             "请先选中一行！");
    }
    else
    {
        model->removeRow(curRow);
        int ok = QMessageBox::warning(this, tr("删除当前用户？"),
                                      tr("你确定删除当前用户吗？"),
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if(ok == QMessageBox::No)
        {
            model->revertAll();
        }
        else
        {
            model->submitAll();
        }
    }
}

void UserManager::on_pushButton_clicked()
{
    sys_add_user *addUser = new sys_add_user;
    addUser->exec();
    model->select();
}

void UserManager::on_pushButton_3_clicked()
{
    //    qDebug()<<ui->tableView->currentIndex().data(0);
    //    qDebug()<<ui->tableView->currentIndex().row();
    //    qDebug()<<ui->tableView->currentIndex().column();
    if(1 == ui->tableView->currentIndex().column())
    {
        if(userCheck(ui->tableView->currentIndex().data(0).toString(), SYS_USER_NUMBER))
        {
            sqLite();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"),
                                 "用户已存在，修改无效！");
            model->revertAll();
        }
    }
    else
    {
        sqLite();
    }

}

void UserManager::on_pushButton_4_clicked()
{
    model->revertAll();
}

void UserManager::sqLite()
{
    if(NULL == ui->tableView->currentIndex().data(0).toString())
    {
        QMessageBox::warning(this, tr("提示"),
                             "请输入需要修改的内容！");
        model->revertAll();
    }
    // 开始事务操作
    model->database().transaction();

    if(model->submitAll())
    {
        model->database().commit();
    }
    else
    {
        model->database().rollback();
        QMessageBox::warning(this, tr("警告"),
                             tr("数据库错误: %1")
                             .arg(model->lastError().text()));
    }
}


void UserManager::on_pushButton_5_clicked()
{
    QString name = ui->lineEdit->text();

    if(NULL == name)
        QMessageBox::warning(this, tr("提示"),
                             "请输入用户名进行查询！");
    else if(!userCheck(name, SYS_USER_NUMBER))
    {
        model->setFilter(QString("User_Name = '%1'")
                         .arg(name));
        model->select();
    }
    else
    {
        QMessageBox::warning(this, tr("提示"),
                             "用户不存在，请更换用户名重试！");
    }

    ui->lineEdit->clear();
}

void UserManager::on_pushButton_6_clicked()
{
    tableModel();
}

void UserManager::tableModel()
{
    model = new QSqlTableModel(this);
    model->setTable("Sys_User");
    // 系统管理只能物流公司使用，即管理系统管理员
    model->setFilter("User_Is_Admin = 1");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("用户编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("用户名"));
    model->setHeaderData(2, Qt::Horizontal, tr("用户密码"));
    model->setHeaderData(3, Qt::Horizontal, tr("用户邮箱"));
    model->setHeaderData(4, Qt::Horizontal, tr("用户身份"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);
    // 不显示用户ID和身份，这些信息不必要
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(4);
    //    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
