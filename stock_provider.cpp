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
void stock_MainWindow::stock_provider_model_init()
{
    stock_provider->setTable("stock_provider");
    stock_provider->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stock_provider->select();
    ui->tableView_3->setModel(stock_provider);
    ui->tableView_3->hideColumn(0);
    ui->tableView_3->hideColumn(1);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stock_provider_product->setTable("stock_provider_product");
    stock_provider_product->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stock_provider_product->select();
    ui->tableView_4->setModel(stock_provider_product);
    ui->tableView_4->hideColumn(0);
    ui->tableView_4->hideColumn(1);
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stock_tableview_3_clicked(stock_provider->index(0,0));
    stock_set_headname();
}
void stock_MainWindow::stock_set_headname()
{
    stock_provider->setHeaderData(2,Qt::Horizontal,tr("供货商名称"));
    stock_provider->setHeaderData(3,Qt::Horizontal,tr("供货商地址"));
    stock_provider_product->setHeaderData(2,Qt::Horizontal,tr("商品名称"));
    stock_provider_product->setHeaderData(3,Qt::Horizontal,tr("商品价格"));
}
void stock_MainWindow::set_provider_visible(bool b)
{
    ui->label_9->setVisible(!b);
    ui->label_10->setVisible(!b);
    ui->label_11->setVisible(!b);
    ui->lineEdit_2->setVisible(!b);
    ui->lineEdit_3->setVisible(!b);
    ui->tableWidget->setVisible(!b);
    ui->pushButton_4->setVisible(!b);
    ui->label_7->setVisible(b);
    ui->label_8->setVisible(b);
    ui->tableView_3->setVisible(b);
    ui->tableView_4->setVisible(b);
    ui->pushButton_7->setVisible(b);
    set_provider_modify(!b);
}
void stock_MainWindow::set_provider_modify(bool b)
{
    ui->pushButton_4->setVisible(b);
    ui->pushButton_5->setVisible(b);
    ui->pushButton_6->setVisible(b);
    if(ui->comboBox_2->currentIndex()==0)
    {
        ui->pushButton_8->setVisible(b);

    }
    else
    {
        ui->pushButton_8->setVisible(false);
    }

}
void stock_MainWindow::on_pushButton_4_clicked()
{
    QString name;
    QString address;
    QSqlQuery query;
    int provider_id;
    int ok;
    int index=ui->comboBox_2->currentIndex();
    switch (index) {
    case 0:
        stock_provider_product->database().transaction();
        if(stock_provider_product->submitAll())
        {
            stock_provider_product->database().commit();
        }
        else
        {
            stock_provider_product->database().rollback();
            qDebug()<<"save fail";
        }
        break;
    case 1:
        name=ui->lineEdit_2->text();
        if(name=="")
        {
            QMessageBox::warning(this,tr("错误"),tr("供应商名称不能为空"));
            return;
        }
        address=ui->lineEdit_3->text();
        if(address=="")
        {
            QMessageBox::warning(this,tr("错误"),tr("供应商地址不能为空"));
            return;
        }
        query.exec(QString("select * from stock_provider where name = '%1'").arg(name));
        qDebug()<<query.lastError();
        if(query.next())
        {
            QMessageBox::warning(this,tr("错误"),tr("供应商已经存在，请到查询页面进行修改"));
            return;
        }
        provider_id=stock_make_id(3);
        ok=QMessageBox::warning(this,tr("提醒"),tr("你想把这个供货商信息共享给其他卖家吗？"),QMessageBox::Yes,QMessageBox::No);
        query.prepare("insert into stock_provider(id,owner_id,name,address) values(?,?,?,?)");
        query.addBindValue(provider_id);
        if(ok!=QMessageBox::Yes)
        {
           query.addBindValue(user_id);
        }
        else
        {
            query.addBindValue(GLOBAL_USER);
        }
        query.addBindValue(name);
        query.addBindValue(address);
        query.exec();
        qDebug()<<query.lastError();
        for(int i=0;i<ui->tableWidget->rowCount();i++)
        {
            int id=stock_make_id(5+i);
            QString name;
            float price;
            if(ui->tableWidget->item(i,0)!=NULL)
            {
                name=ui->tableWidget->item(i,0)->text();
            }
            else
            {
                QMessageBox::warning(this,tr("错误"),tr("商品名称不能为空"));
                return;
            }
            if(ui->tableWidget->item(i,1)!=NULL)
            {
                price=ui->tableWidget->item(i,1)->text().toFloat();
            }
            else
            {
                QMessageBox::warning(this,tr("错误"),tr("商品价格不能为空"));
                return;
            }
             query.prepare("insert into stock_provider_product(id,provider_id,name,price) values(?,?,?,?)");
             query.addBindValue(id);
             query.addBindValue(provider_id);
             query.addBindValue(name);
             query.addBindValue(price);
             query.exec();
             qDebug()<<query.lastError();
        }
        break;
    default:
        break;
    }
}
void stock_MainWindow::on_pushButton_5_clicked()
{

    int index=ui->comboBox_2->currentIndex();
    int rowNum;
    int id;
    int provider_id;
    int provider_row=ui->tableView_3->currentIndex().row();
    switch (index) {
    case 0:
        rowNum=stock_provider_product->rowCount();
        id=stock_make_id(5);
        stock_provider_product->insertRow(rowNum);
        provider_id=stock_provider->data(stock_provider->index(provider_row,0)).toInt();
        stock_provider_product->setData(stock_provider_product->index(rowNum,0),id);
        stock_provider_product->setData(stock_provider_product->index(rowNum,1),provider_id);
        break;
    case 1:
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);break;
    default:
        break;
    }
}
void stock_MainWindow::on_pushButton_6_clicked()
{
    int index=ui->comboBox_2->currentIndex();
    int row;
    int ok;
    switch (index) {
    case 0:
        row=ui->tableView_4->currentIndex().row();
        ok=QMessageBox::warning(this,tr("删除当前商品！"),tr("你确定删除当前商品吗？"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::Yes)
        {
            stock_provider_product->removeRow(row);
            stock_provider_product->submitAll();
        }
        break;
    case 1:
        row=ui->tableWidget->currentRow();
        if(row!=-1)
        {
            ui->tableWidget->removeRow(row);
        }
        break;
    default:
        break;
    }
}
void stock_MainWindow::on_pushButton_8_clicked()
{
    int row=ui->tableView_3->currentIndex().row();
    int provider_id=stock_provider->data(stock_provider->index(row,0)).toInt();
    QMessageBox box(QMessageBox::Warning,tr("删除当前供货商！"),tr("你确定删除当前供货商吗？"));
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setButtonText(QMessageBox::Yes,tr("确认"));
    box.setButtonText(QMessageBox::No,tr("取消"));
    int ok= box.exec();
    if(ok==QMessageBox::Yes)
    {
        QSqlQuery query;
        query.exec(QString("delete from stock_provider_product where provider_id=%1").arg(provider_id));
        qDebug()<<query.lastError();
        stock_provider->removeRow(row);
        stock_provider->submitAll();
        stock_refresh_provider();
    }
}
void stock_MainWindow::on_pushButton_7_clicked()
{
    if(!ui->pushButton_4->isVisible())
    {
         set_provider_modify(true);
         ui->tableView_3->setEditTriggers(QAbstractItemView::AllEditTriggers);
         ui->tableView_4->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    else
    {
        set_provider_modify(false);
        ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
void stock_MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        set_provider_visible(true);
        stock_refresh_provider();
        break;
    case 1:
        set_provider_visible(false);
        break;
    default:
        break;
    }
}
void stock_MainWindow::stock_refresh_provider()
{
    stock_provider->select();
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stock_tableview_3_clicked(stock_provider->index(0,0));
    stock_set_headname();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
}
void stock_MainWindow::stock_tableview_3_clicked(const QModelIndex&index)
{

    int provider_id=stock_provider->data(stock_provider->index(index.row(),0)).toInt();
    stock_provider_product->setFilter(tr("provider_id='%1'").arg(provider_id));
    stock_provider_product->select();

}
