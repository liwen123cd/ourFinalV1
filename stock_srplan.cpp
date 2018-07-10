#include "stock_mainwindow.h"
#include "ui_stock_mainwindow.h"
#include "stock_dialog.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QSqlError>
void stock_MainWindow::stock_srplan_init()
{
    stock_srplan_basic->setQuery(tr("select * from stock_plan p where not exists(select * from stock_plan_detail d where d.plan_id=p.id and (d.state='进货未完成'or d.state='未完成'))"));
    ui->pushButton_10->setVisible(false);
    ui->pushButton_11->setVisible(false);
    stock_srplan_init_aux(0);

}
void stock_MainWindow::stock_srplan_init_aux(int row)
{
    stock_srplan_basic->setHeaderData(0,Qt::Horizontal,tr("进货计划编号"));
    stock_srplan_basic->setHeaderData(2,Qt::Horizontal,tr("进货计划总价格"));
    stock_srplan_basic->setHeaderData(3,Qt::Horizontal,tr("进货计划总运费"));
    stock_srplan_basic->setHeaderData(4,Qt::Horizontal,tr("进货计划创建时间"));
    ui->tableView_6->setModel(stock_srplan_basic);
    ui->tableView_6->setCurrentIndex(stock_srplan_basic->index(row,0));
    ui->tableView_6->hideColumn(1);
    ui->tableView_6->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setModel(stock_srplan_detail);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    stock_tableview_6_clicked(stock_srplan_basic->index(row,0));
}
void stock_MainWindow::stock_srplan_noncompleted(int row)
{
    stock_srplan_basic->setQuery(tr("select * from stock_plan p where exists(select * from stock_plan_detail d where d.plan_id=p.id and (d.state='进货未完成'or d.state='未完成'))"));
    ui->pushButton_10->setVisible(true);
    ui->pushButton_11->setVisible(true);
    stock_srplan_init_aux(row);
}
void stock_MainWindow::stock_srplan_canceled()
{
    //ui->tableView_6->setModel(stock_srplan_basic);
    ui->pushButton_10->setVisible(false);
    ui->pushButton_11->setVisible(false);
    stock_srplan_basic->setQuery("select * from stock_canceledplan");
    stock_srplan_canceled_aux();
}
void stock_MainWindow::stock_srplan_canceled_aux()
{
    stock_srplan_basic->setHeaderData(0,Qt::Horizontal,tr("已取消计划编号"));
    stock_srplan_basic->setHeaderData(2,Qt::Horizontal,tr("已取消计划总价格"));
    stock_srplan_basic->setHeaderData(3,Qt::Horizontal,tr("最近取消时间"));
    ui->tableView_6->setModel(stock_srplan_basic);
    ui->tableView_6->hideColumn(1);
    ui->tableView_6->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setModel(stock_srplan_detail);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    stock_tableview_6_clicked(stock_srplan_basic->index(0,0));
}

void stock_MainWindow::stock_tableview_6_clicked(const QModelIndex &index)
{
    int cur_i=ui->comboBox->currentIndex();
    int plan_id=stock_srplan_basic->data(stock_srplan_basic->index(index.row(),0)).toInt();
    switch (cur_i) {
    case 0:
    case 1:
        stock_srplan_detail->setQuery(tr("select * from stock_plan_detail where plan_id=%1").arg(plan_id));
        stock_srplan_detail->setHeaderData(2,Qt::Horizontal,tr("商品编号"));
        stock_srplan_detail->setHeaderData(3,Qt::Horizontal,tr("商品进货数量"));
        stock_srplan_detail->setHeaderData(4,Qt::Horizontal,tr("商品进货总价"));
        stock_srplan_detail->setHeaderData(5,Qt::Horizontal,tr("商品进货状态"));
        ui->tableView->hideColumn(0);
        ui->tableView->hideColumn(1);
        break;
    case 2:
        stock_srplan_detail->setQuery(QString("select * from stock_canceledplan_detail where plan_id=%1").arg(plan_id));
        stock_srplan_detail->setHeaderData(2,Qt::Horizontal,tr("商品编号"));
        stock_srplan_detail->setHeaderData(3,Qt::Horizontal,tr("商品进货数量"));
        stock_srplan_detail->setHeaderData(4,Qt::Horizontal,tr("商品进货总价"));
        break;
    default:
        break;
    }
}
void  stock_MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        stock_srplan_init();
        break;
    case 1:
        stock_srplan_noncompleted(0);
        break;
    case 2:
        stock_srplan_canceled();
        break;
    default:
        break;
    }
}
/*这个方法负责删除一个完整的进货计划，要做如下的事：
 * 1.如果没有选中一个计划，则提示没有选中，然后退出。
 * 2.判断这个计划是否有已经完成的详细条目，如果有则提示出错，然后退出。
 * 3.若没有已经完成的，就可以通过调用删除条目的函数来删除全部条目来达到目的。
 */
void stock_MainWindow::on_pushButton_10_clicked()
{
    int rowNum=ui->tableView_6->currentIndex().row();
    if(rowNum<0)
    {
        QMessageBox::warning(this,tr("取消失败"),tr("请点击一个要取消的计划！！！"));
        return;
    }
    int plan_id=stock_srplan_basic->data(stock_srplan_basic->index(rowNum,0)).toInt();
    QSqlQuery query;
    query.exec(QString("select * from stock_plan_detail where state='进货已完成' and plan_id=%1").arg(plan_id));
    qDebug()<<query.lastError();
    if(query.next())
    {
        QMessageBox::warning(this,tr("取消失败"),tr("这个计划有的条目已经进货完成，不能取消！！！"));
        return;
    }
    QMessageBox box(QMessageBox::Warning,tr("删除当前进货计划！"),tr("你确定删除当前进货计划吗？"));
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setButtonText(QMessageBox::Yes,tr("确认"));
    box.setButtonText(QMessageBox::No,tr("取消"));
    int ok= box.exec();
    if(ok!=QMessageBox::Yes) return;
    int cnt=stock_srplan_detail->rowCount();//获取总共要删除的条目数
    for(int i=0;i<cnt;i++)
    {
        stock_real_cancel(0);//都传入0的原因是，删除第一个条目后，第二个条目就变为了第一个条目了。因此，永远都是删除位置在0的条目。
    }
    //完事

}

/*这个方法要做的几件事：
 * 1.删除详细表的条目，并且往取消详细表插入一条相应的条目
 * 2.如果这个计划的详细表已经为空，那么就要把基本信息表的对应条目也要删除
 * 3.要修改相应的基本信息表的信息和取消基本信息表的信息，如果取消基本信息表的相应条目还没有生成就往这个表插入一条相应的条目
 * 4.记得要修改相应的库存空间
 * 5.对了，我还漏了一个判断，就是如果这个条目已经完成了，就不能取消了。
 */
void stock_MainWindow::on_pushButton_11_clicked()
{
    int rowNum=ui->tableView->currentIndex().row();
    if(rowNum<0)
    {
        QMessageBox::warning(this,tr("取消失败"),tr("请点击一条要取消的条目！！！"));
        return;
    }
    QString state=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,5)).toString();
    if(state==tr("进货已完成"))
    {
        QMessageBox::warning(this,tr("取消失败"),tr("这个条目已经进货完成，无法取消！！！"));
        return;
    }
    QMessageBox box(QMessageBox::Warning,tr("删除当前进货条目！"),tr("你确定删除当前进货条目吗？"));
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setButtonText(QMessageBox::Yes,tr("确认"));
    box.setButtonText(QMessageBox::No,tr("取消"));
    int ok= box.exec();
    if(ok!=QMessageBox::Yes) return;
    stock_real_cancel(rowNum);
}
void stock_MainWindow::stock_real_cancel(int rowNum)
{
    int id=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,0)).toInt();
    int plan_id=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,1)).toInt();
    int product_id=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,2)).toInt();
    int cnt=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,3)).toInt();
    storage_space+=cnt;
    float price=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,4)).toFloat();
    QSqlQuery query;
    query.exec(QString("delete from stock_plan_detail where id=%1").arg(id));
    qDebug()<<query.lastError();
    query.exec(QString("select * from stock_plan_detail where plan_id=%1").arg(plan_id));
    qDebug()<<query.lastError();
    if(!query.next())//若已经没有详细记录，则从基本信息表中删除
    {
        query.exec(QString("delete from stock_plan where id=%1").arg(plan_id));
        qDebug()<<query.lastError();
        stock_srplan_noncompleted(0);
    }
    else//若还有，则修改基本信息表的内容...数据库里怎么把float存成double。还有我忘记next了，所以总是得到错误的值
    {
        query.exec(QString("select * from stock_plan where id=%1").arg(plan_id));
        query.next();
        qDebug()<<query.lastError();
        float total_price=query.value(2).toFloat();
        float fee=query.value(3).toFloat();
        total_price-=price;
        fee-=(price*0.01);
        query.exec(QString("update stock_plan set money=%1,fee=%2 where id=%3").arg(total_price).arg(fee).arg(plan_id));
        qDebug()<<query.lastError();
        stock_srplan_noncompleted(ui->tableView_6->currentIndex().row());
    }
    query.exec(QString("insert into stock_canceledplan_detail(id,plan_id,product_id,cnt,price) values(%1,%2,%3,%4,%5)").arg(id).arg(plan_id).arg(product_id).arg(cnt).arg(price));
    qDebug()<<query.lastError();
    query.exec(QString("select * from stock_canceledplan where id=%1").arg(plan_id));
    qDebug()<<query.lastError();
    if(query.next())//若删除基础表中已有相关记录，则修改其信息
    {
        float total_price=query.value(2).toFloat();
        total_price+=price;
        query.prepare("update stock_canceledplan set money=?,time=? where id=?");
        query.addBindValue(total_price);
        query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.addBindValue(plan_id);
        query.exec();
        qDebug()<<query.lastError();
    }
    else//若还没有，则添加一条信息。因为只有是卖家才能改，而且卖家只能改自己的进货计划，所以seller_id就直接用user_id了
    {
        query.prepare("insert into stock_canceledplan(id,seller_id,money,time) values(?,?,?,?)");
        query.addBindValue(plan_id);
        query.addBindValue(user_id);
        query.addBindValue(price);
        query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
        qDebug()<<query.lastError();
    }
    //完事

}
void stock_MainWindow::on_pushButton_clicked()
{
    bool ok=true;
    QString s=ui->lineEdit->text();
    int cur_id=ui->comboBox->currentIndex();
    if(s=="")
    {
       on_comboBox_currentIndexChanged(cur_id);
       return;
    }
    int plan_id=ui->lineEdit->text().toInt(&ok);
    if(!ok||plan_id<0)
    {
        QMessageBox::warning(this,tr("查询失败"),tr("计划编号必须是正整数！！！"));
        return;
    }
    switch (cur_id) {
    case 0:
        stock_srplan_basic->setQuery(tr("select * from stock_plan p where p.id=%1 and not exists(select * from stock_plan_detail d where d.plan_id=p.id and (d.state='进货未完成'or d.state='未完成'))").arg(plan_id));
        stock_srplan_init_aux(0);
        break;
    case 1:
        stock_srplan_basic->setQuery(tr("select * from stock_plan p where p.id=%1 and exists(select * from stock_plan_detail d where d.plan_id=p.id and (d.state='进货未完成'or d.state='未完成'))").arg(plan_id));
        stock_srplan_init_aux(0);
        break;
    case 2:
        stock_srplan_basic->setQuery(tr("select * from stock_canceledplan where id=%1").arg(plan_id));
        stock_srplan_canceled_aux();
        break;
    default:
        break;
    }

}
void stock_MainWindow::stock_tableview_doubleclicked(const QModelIndex &index)
{
    int rowNum=index.row();
    int product_id=stock_srplan_detail->data(stock_srplan_detail->index(rowNum,2)).toInt();
    QSqlQuery query;
    query.exec(tr("select * from stock_provider_product where id=%1").arg(product_id));
    query.next();
    int provider_id=query.value(1).toInt();
    QString product_name=query.value(2).toString();
    float price=query.value(3).toFloat();
    query.exec(tr("select * from stock_provider where id=%1").arg(provider_id));
    query.next();
    QString provider_name=query.value(2).toString();
    QString address=query.value(3).toString();
    stock_Dialog *dialog=new stock_Dialog(this);
    dialog->set_content(product_name,price,provider_name,address);
    dialog->setModal(false);
    dialog->show();

}
