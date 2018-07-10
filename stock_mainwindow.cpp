#include "stock_mainwindow.h"
#include "ui_stock_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlError>
/*与库存容量方面的交互必须想清楚，不要有疏漏。主要就是添加进货时减库存，取消进货时加库存。
 * 每次进入进货计划制定界面就要读取一次库存容量，不过如果这个界面还有上次没制定完的计划就另当别论。
 * 感觉如果没有制定完计划，就必须要记住当前计划所使用的库存容量，因为仓库那边传回来的剩余容量会变。s
 *
 */
stock_MainWindow::stock_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::stock_MainWindow)
{
    ui->setupUi(this);
    user_id=1;
    storage_space=100;
    stock_mkplan_provider=new QSqlQueryModel(this);
    stock_mkplan_product=new QSqlQueryModel(this);
    stock_srplan_basic=new QSqlQueryModel(this);
    stock_srplan_detail=new QSqlQueryModel(this);
    stock_provider=new QSqlTableModel(this);
    stock_provider_product=new QSqlTableModel(this);
    stock_provider_model_init();
    stock_mkplan_init();
    stock_srplan_init();
    ui->comboBox->addItem(tr("已完成订单"));
    ui->comboBox->addItem(tr("未完成订单"));
    ui->comboBox->addItem(tr("已取消订单"));
    ui->comboBox_2->addItem(tr("供货商信息查询"));
    ui->comboBox_2->addItem(tr("新增供货商信息"));
    ui->tableWidget->setColumnCount(2);
    QStringList header;
    header<<tr("商品名称（输入字符串）")<<tr("商品价格（输入实数）");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget_2->setColumnCount(4);
    ui->tableWidget_2->hideColumn(0);
    header.clear();
    header<<tr("商品编号")<<tr("商品名称")<<tr("商品进货数量")<<tr("商品总价");
    ui->tableWidget_2->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView_3->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_3->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView_4->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_4->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    set_provider_visible(true);
    connect(ui->tableView_3,SIGNAL(pressed(QModelIndex)),this,SLOT(stock_tableview_3_clicked(QModelIndex)));
    connect(ui->tableView_2,SIGNAL(pressed(QModelIndex)),this,SLOT(stock_tableview_2_clicked(QModelIndex)));
    connect(ui->tableView_5,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(stock_tableview_5_doubleclicked(QModelIndex)));
    connect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
    connect(ui->tableWidget_2,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_pressed(QTableWidgetItem*)));
    connect(ui->tableView_6,SIGNAL(pressed(QModelIndex)),this,SLOT(stock_tableview_6_clicked(QModelIndex)));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(on_tabWidget_currentChanged(int)));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(stock_tableview_doubleclicked(QModelIndex)));
    ui->lcdNumber->setNumDigits(4);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_2->setDigitCount(10);
    ui->lcdNumber_2->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_2->setStyleSheet("border:1px solid black;color: blue;background: silver");
    ui->lcdNumber_3->setDigitCount(10);
    ui->lcdNumber_3->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber_3->setStyleSheet("border:1px solid black;color: red;background: silver");
    this->setCentralWidget(ui->horizontalLayoutWidget);

 }

stock_MainWindow::~stock_MainWindow()
{
    delete ui;
}
void stock_MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
        ui->comboBox->setCurrentIndex(0);
        stock_srplan_init();
        break;
    case 1:
        stock_mkplan_init();
        break;
    case 2:
        stock_provider_model_init();
    default:
        break;
    }
}

