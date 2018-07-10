#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globaldata.h"
#include "usermanager.h"
#include "staffmanager.h"
#include "storagedialog.h"
#include "sale_widget.h"
#include "stock_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(0 == Data::is_admin)
    {
        ui->pushButton->setVisible(false);
        ui->pushButton_3->setVisible(false);
    }
    //qDebug()<<User::name;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    UserManager *um = new UserManager;
//    this->hide();
    um->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    StaffManager *sm = new StaffManager;
    sm->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    StorageDialog *sd = new StorageDialog;
    sd->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    Sale_Widget *sw = new Sale_Widget;
    sw->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    stock_MainWindow *sm = new stock_MainWindow;
    sm->show();
}
