#include "stock_mainwindow.h"
#include "ui_stock_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlError>
static int prev_num=0;
static float total_price=0;
static float fee=0;
void stock_MainWindow::set_restSpace()
{

    ui->lcdNumber->display(storage_space);
}
void stock_MainWindow::set_price_fee()
{
    ui->lcdNumber_2->display(total_price);
    ui->lcdNumber_3->display(fee);
}
void stock_MainWindow::stock_mkplan_init()
{
    stock_mkplan_provider->setQuery("select * from stock_provider");
    stock_mkplan_provider->setHeaderData(2,Qt::Horizontal,tr("供货商名称"));
    stock_mkplan_provider->setHeaderData(3,Qt::Horizontal,tr("供货商地址"));
    ui->tableView_2->setModel(stock_mkplan_provider);
    ui->tableView_2->hideColumn(0);
    ui->tableView_2->hideColumn(1);
    ui->tableView_2->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView_5->setModel(stock_mkplan_product);
    ui->tableView_5->setSelectionBehavior(QTableView::SelectRows);
    stock_tableview_2_clicked(stock_mkplan_provider->index(0,0));
    set_restSpace();


}
int stock_MainWindow::stock_make_id(int type)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int id=qrand();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    id>>=(qrand()%type);
    return id;
}
void stock_MainWindow::stock_tableview_2_clicked(const QModelIndex&index)
{
    int provider_id=stock_mkplan_provider->data(stock_mkplan_provider->index(index.row(),0)).toInt();
    stock_mkplan_product->setQuery(tr("select * from stock_provider_product where provider_id = %1").arg(provider_id));
    ui->tableView_5->hideColumn(0);
    ui->tableView_5->hideColumn(1);
    stock_mkplan_product->setHeaderData(2,Qt::Horizontal,tr("商品名称"));
    stock_mkplan_product->setHeaderData(3,Qt::Horizontal,tr("商品价格"));
}
void stock_MainWindow::stock_tableview_5_doubleclicked(const QModelIndex&index)
{
    QInputDialog dialog(this);
    dialog.setModal(false);
    dialog.setWindowTitle(tr("输入商品的进货数量"));
    dialog.setLabelText(tr("请输入该商品进货的数量："));
    dialog.setOkButtonText(tr("确认"));
    dialog.setCancelButtonText(tr("取消"));
    dialog.setInputMode(QInputDialog::IntInput);
    dialog.setIntMinimum(1);
    dialog.setGeometry(800,500,300,300);
    if(dialog.exec()!=QDialog::Accepted)
    {
        return;
    }
    int num=dialog.intValue();
    stock_mkplan_change(index,num);
}
void stock_MainWindow::on_pushButton_9_clicked()
{
   if(ui->tableView_5->currentIndex().row()<0)
   {
       QMessageBox::warning(this,tr("错误"),tr("请选中一个商品!!!"));
       return;
   }
   stock_mkplan_change(ui->tableView_5->currentIndex(),1);
}
void stock_MainWindow::stock_tablewidget_2_changed(QTableWidgetItem*item)
{
    disconnect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
    int x=item->row();
    int product_id=ui->tableWidget_2->item(x,0)->text().toInt();
    float unit_price=stock_getProduct_price(product_id);
    int cur_num=ui->tableWidget_2->item(x,2)->text().toInt();
    if(cur_num<=0)
    {
        QMessageBox::warning(this,tr("错误"),tr("进货数量必须是正数！！！"));
        ui->tableWidget_2->setItem(x,2,new QTableWidgetItem(tr("%1").arg(prev_num)));
        ui->tableWidget_2->setItem(x,3,new QTableWidgetItem(tr("%1").arg(prev_num*unit_price)));
        stock_mkplan_edit(x);
        connect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
        return;
    }
    int rest_space=storage_space;
    int diff=cur_num-prev_num;
    if(diff>rest_space)
    {
        QMessageBox::warning(this,tr("添加失败"),tr("你的仓库剩余空间不足！！！"));
        ui->tableWidget_2->setItem(x,2,new QTableWidgetItem(tr("%1").arg(prev_num)));
        ui->tableWidget_2->setItem(x,3,new QTableWidgetItem(tr("%1").arg(prev_num*unit_price)));
        stock_mkplan_edit(x);
        connect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
        return;
    }
    storage_space-=diff;
    set_restSpace();
    ui->tableWidget_2->setItem(x,3,new QTableWidgetItem(tr("%1").arg(cur_num*unit_price)));
    total_price+=(diff*unit_price);
    fee+=(diff*unit_price*0.01);
    stock_mkplan_edit(x);
    set_price_fee();
    connect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
}

void stock_MainWindow::stock_tablewidget_2_pressed(QTableWidgetItem *item)
{
    prev_num=ui->tableWidget_2->item(item->row(),2)->text().toInt();
}

void stock_MainWindow::stock_mkplan_change(const QModelIndex&index,int num)
{
    int rest_space=storage_space;
    if(num>rest_space)
    {
        QMessageBox::warning(this,tr("添加失败"),tr("你的仓库剩余空间不足！！！"));
        return;
    }
    storage_space-=num;
    set_restSpace();
    disconnect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
    int product_id=stock_mkplan_product->data(stock_mkplan_product->index(index.row(),0)).toInt();
    float unit_price=stock_getProduct_price(product_id);
    int rowNum;
    if(choosen_id.find(product_id)!=choosen_id.end())
    {
        rowNum=choosen_id[product_id];
    }
    else
    {
        rowNum=ui->tableWidget_2->rowCount();
        choosen_id[product_id]=rowNum;
        ui->tableWidget_2->setRowCount(rowNum+1);
        QString product_name=stock_mkplan_product->data(stock_mkplan_product->index(index.row(),2)).toString();
        ui->tableWidget_2->setItem(rowNum,0,new QTableWidgetItem(tr("%1").arg(product_id)));
        ui->tableWidget_2->setItem(rowNum,1,new QTableWidgetItem(product_name));
        ui->tableWidget_2->setItem(rowNum,2,new QTableWidgetItem(tr("%1").arg(0)));
        ui->tableWidget_2->setItem(rowNum,3,new QTableWidgetItem(tr("%1").arg(0.0)));
    }
    int cur_num=ui->tableWidget_2->item(rowNum,2)->text().toInt();
    float cur_price=ui->tableWidget_2->item(rowNum,3)->text().toFloat();
    ui->tableWidget_2->setItem(rowNum,2,new QTableWidgetItem(tr("%1").arg(cur_num+num)));
    ui->tableWidget_2->setItem(rowNum,3,new QTableWidgetItem(tr("%1").arg(cur_price+num*unit_price)));
    total_price+=(num*unit_price);
    fee+=(num*unit_price*0.01);
    set_price_fee();
    stock_mkplan_edit(rowNum);
    connect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(stock_tablewidget_2_changed(QTableWidgetItem*)));
}

void stock_MainWindow::stock_mkplan_edit(int rowNum)
{
    ui->tableWidget_2->item(rowNum,0)->setFlags(Qt::NoItemFlags);
    ui->tableWidget_2->item(rowNum,1)->setFlags(Qt::NoItemFlags);
    ui->tableWidget_2->item(rowNum,3)->setFlags(Qt::NoItemFlags);
}
void stock_MainWindow::stock_mkplan_create()
{
    int plan_id=stock_make_id(3);
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("insert into stock_plan(id,seller_id,money,fee,time) values(?,?,?,?,?)");
    query.addBindValue(plan_id);
    query.addBindValue(user_id);
    query.addBindValue(total_price);
    query.addBindValue(fee);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    if(!query.exec())
    {
        qDebug()<<tr("创建订单基本信息失败");
        QMessageBox::warning(this,tr("生成订单失败"),tr("由于未知的原因，进货订单生成失败!"));
        qDebug()<<query.lastError();
        QSqlDatabase::database().rollback();
        return;
    }
    QString state(tr("进货未完成"));
    for(int i=0;i<ui->tableWidget_2->rowCount();i++)
    {
        int id=stock_make_id(5+i);
        int product_id=ui->tableWidget_2->item(i,0)->text().toInt();
        int cnt=ui->tableWidget_2->item(i,2)->text().toInt();
        float price=ui->tableWidget_2->item(i,3)->text().toFloat();
        query.prepare("insert into stock_plan_detail(id,plan_id,product_id,cnt,price,state) values(?,?,?,?,?,?)");
        query.addBindValue(id);
        query.addBindValue(plan_id);
        query.addBindValue(product_id);
        query.addBindValue(cnt);
        query.addBindValue(price);
        query.addBindValue(state);
        if(!query.exec())
        {
            qDebug()<<tr("创建订单详细失败");
            QMessageBox::warning(this,tr("生成订单失败"),tr("由于未知的原因，进货订单生成失败!"));
            qDebug()<<query.lastError();
            QSqlDatabase::database().rollback();
            return;
        }
    }
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->clearContents();
    total_price=0;
    fee=0;
    set_price_fee();
    QSqlDatabase::database().commit();
}
void stock_MainWindow::on_pushButton_3_clicked()
{
    int rowCount=ui->tableWidget_2->rowCount();
    if(!rowCount)
    {
        QMessageBox::warning(this,tr("生成订单失败"),tr("您的进货清单为空，不能生成订单！！！"));
        return;
    }
    QMessageBox box(QMessageBox::Warning,tr("确认生成订单！"),tr("你确认要生成当前的订单吗？"));
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setButtonText(QMessageBox::Yes,tr("确认"));
    box.setButtonText(QMessageBox::No,tr("取消"));
    int ok= box.exec();
    if(ok==QMessageBox::Yes)
    stock_mkplan_create();
}

void stock_MainWindow::on_pushButton_2_clicked()
{
    int rowNum=ui->tableWidget_2->currentRow();
    if(rowNum<0)
    {
        QMessageBox::warning(this,tr("错误"),tr("请选中一条记录!!!"));
        return;
    }
    int num=ui->tableWidget_2->item(rowNum,2)->text().toInt();
    int product_id=ui->tableWidget_2->item(rowNum,0)->text().toInt();
    float unit_price=stock_getProduct_price(product_id);
    choosen_id.erase(product_id);
    ui->tableWidget_2->removeRow(rowNum);
    total_price-=(num*unit_price);
    fee-=(num*unit_price*0.01);
    set_price_fee();
    storage_space+=num;
    set_restSpace();
}
float stock_MainWindow::stock_getProduct_price(int product_id)
{
    QSqlQuery query;
    query.exec(QString("select price from stock_provider_product where id=%1").arg(product_id));
    query.next();
    qDebug()<<query.lastError();
    return query.value(0).toFloat();
}
