#include "storagedialog.h"
#include "ui_storagedialog.h"
#include "dbconn.h"
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "querydialog.h"
#include "mysqlquerymodel.h"
#include "insertdialog.h"
#include "myitemdelegate.h"
#include "storagemanage.h"

int is_admin = 1;

StorageDialog::StorageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StorageDialog)
{
    ui->setupUi(this);
    queryDialog = new QueryDialog(this);
    insertDialog = new InsertDialog(this);
    db = QSqlDatabase::database();
    QSqlQuery query(db);

    // 设置仓库combox数值
    query.exec("select storageName from Storage_info");
    while (query.next()) {
        ui->comboBox_sto->addItem(query.value(0).toString());
    }

    // 处理model
    model = new MySqlQueryModel;
    model->setQuery("select * from Main_view", db);
    if (model->lastError().isValid()) qDebug() << model->lastError().text();
    model->setHeaderData(0, Qt::Horizontal, "仓库");
    model->setHeaderData(1, Qt::Horizontal, "卖家");
    model->setHeaderData(2, Qt::Horizontal, "商品");
    model->setHeaderData(3, Qt::Horizontal, "数量");
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new MyItemDelegate(this));
    //ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->label_record_num->setText("共" + QString::number(model->rowCount()) + "条记录");

    // 信号与槽的连接
    connect(queryDialog, SIGNAL(sendMsg(QString,QString)),
            this, SLOT(getQuery(QString,QString)));
    connect(insertDialog, SIGNAL(sendMsg(QString,QString,QString)),
            this, SLOT(getInsert(QString,QString,QString)));

    // 判断是否为管理员
    if (0 == is_admin) {
        ui->pushBtn_query->setEnabled(false);
        ui->pushBtn_query_all->setEnabled(false);
        ui->tabWidget->setTabEnabled(1, false);
    }
}

StorageDialog::~StorageDialog()
{
    delete ui;
    delete model;
}

// 仓库选择
void StorageDialog::on_comboBox_sto_activated(const QString &stoName)
{
    if ("全部" == stoName) {
        model->setQuery("select * from Main_view", db);
        int rowCount = model->rowCount();
        ui->label_remain_num->setText("-/-");
        ui->label_record_num->setText("共" + QString::number(rowCount) + "条记录");
        return;
    }

    // 在仓库Combox中选中仓库后，显示相应信息
    QString stoID;
    QSqlQuery query(db);
    model->setQuery(QString("select * from Main_view "
                    "where storageName='%1'").arg(stoName), db);
    query.exec(QString("select storageID from Storage_info "
               "where storageName='%1'").arg(stoName));
    if (query.next()) {
        stoID = query.value(0).toString();
    }
    query.exec(QString("select remain,capacity from Storage_info "
               "where storageID=%1").arg(stoID));
    if (query.next()) {
        QString remain, record;
        remain += query.value(0).toString() + "/" + query.value(1).toString();
        record += "共" + query.value(0).toString() + "条记录";
        ui->label_remain_num->setText(remain);
        ui->label_record_num->setText(record);
    }

}

// 查询所有
void StorageDialog::on_pushBtn_query_all_clicked()
{
    model->setQuery("select * from Main_view", db);
    if (model->lastError().isValid()) qDebug() << model->lastError().text();
}

// 查询
void StorageDialog::on_pushBtn_query_clicked()
{
    queryDialog->show();
}

void StorageDialog::getQuery(QString option, QString content)
{
    if ("卖家姓名" == option) {
        model->setQuery(QString("select * from Main_view "
                        "where sellerName='%1'").arg(content), db);
    } else {
        model->setQuery(QString("select * from Main_view "
                        "where productName='%1'").arg(content), db);
    }
    QString record;
    record += "共" + QString::number(model->rowCount()) + "条记录";
    ui->label_record_num->setText(record);
    ui->label_remain_num->setText("-/-");
}

// 插入记录
void StorageDialog::on_pushBtn_insert_clicked()
{
    insertDialog->show();
}

void StorageDialog::getInsert(QString stoName, QString proName, QString amount)
{
    QString stoID;
    QString proID;
    QSqlQuery query(db);

    // 检查所填仓库是否存在
    query.exec(QString("select storageID from Storage_info "
               "where storageName='%1'").arg(stoName));
    if (!query.next()) {
        ui->label_prompt_1->setText("提示：所填仓库不存在！");
        return;
    }
    stoID = query.value(0).toString();

    // 检查所填商品是否存在
    query.exec(QString("select productID from product "
               "where productName='%1'").arg(proName));
    if (!query.next()) {
        ui->label_prompt_1->setText("提示：所填商品不存在！");
        return;
    }
    proID = query.value(0).toString();

    // 插入至数据库
    query.exec(QString("insert into Storage_product "
               "values(%1,%2,%3)").arg(stoID, proID, amount));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError().text();
        ui->label_prompt_1->setText("提示：添加失败！");
    } else {
        ui->label_prompt_1->setText("提示：添加记录成功！");
    }
    model->setQuery("select * from Main_view", db);
}

// 删除一条记录
void StorageDialog::on_pushBtn_del_clicked()
{
    int ok = QMessageBox::warning(this, "删除当前行", "是否删除？",
                                  QMessageBox::Yes, QMessageBox::No);
    if (ok == QMessageBox::No) {
        ui->label_prompt_1->setText("提示：操作已取消！");
        return;
    }
    QString stoID;
    QString proID;
    QModelIndex curIndex = ui->tableView->currentIndex();
    QString stoName = model->data(model->index(curIndex.row(), 0)).toString();
    QString proName = model->data(model->index(curIndex.row(), 2)).toString();
    QSqlQuery query(db);
    query.exec(QString("select storageID from Storage_info "
               "where storageName='%1'").arg(stoName));
    query.next();
    stoID = query.value(0).toString();

    query.exec(QString("select productID from Product "
               "where productName='%1'").arg(proName));
    query.next();
    proID = query.value(0).toString();

    query.exec(QString("delete from Storage_product "
               "where storageID=%1 and productID=%2").arg(stoID, proID));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError().text();
        ui->label_prompt_1->setText("提示：删除失败！");
    } else {
        ui->label_prompt_1->setText("提示：删除记录成功！");
    }
    model->setQuery("select * from Main_view", db);
}

// 出库入库选择
void StorageDialog::on_comboBox_op_activated(const QString &choice)
{
    if ("出库" == choice) {
        ui->label_product->hide();
        ui->lineEdit_product->hide();
        ui->label_seller->hide();
        ui->lineEdit_seller->hide();
        ui->label_amount->hide();
        ui->spinBox_amount->hide();
    } else if ("入库" == choice){
        ui->label_product->show();
        ui->lineEdit_product->show();
        ui->label_seller->show();
        ui->lineEdit_seller->show();
        ui->label_amount->show();
        ui->spinBox_amount->show();
    }
}

// 订单状态确认
void StorageDialog::on_pushBtn_confirm_clicked()
{
    // 出库操作
    if (ui->comboBox_op->currentText() == "出库") {
        QString orderID = ui->lineEdit_record->text();
        // Sale_Widget::Sale_State_out(orderID);
        ui->label_prompt_2->setText("提示：出库成功!");
    } else {
        // 入库操作
        if (StorageManage::tempOccupied <= 0) {
            ui->label_prompt_2->setText("提示：无未完成进货订单!");
            return;
        }
        QString orderID = ui->lineEdit_record->text();
        QString productID = ui->lineEdit_product->text();
        QString sellerID = ui->lineEdit_seller->text();
        int amount = ui->spinBox_amount->text().toInt();
        // 此处调用接口changeInOrderState(orderID, productID);
        QSqlQuery query(db);
        QSqlQuery modify(db);
        int dec = amount;
        query.exec(QString("select storageID from Storage_info "
                           "where sellerID=%1").arg(sellerID));
        while (dec > 0 && query.next()) {
            QString stoID = query.value(0).toString();
            modify.exec(QString("select remain from Storage_info "
                                "where storageID=%1").arg(stoID));
            modify.next();
            int remain = modify.value(0).toInt();
            // 该仓库剩余空间为0
            if (remain == 0) {
                continue;
            }
            // 该仓库空间小于或等于商品数量
            if (remain <= dec) {
                // 修改仓库空间信息
                modify.exec(QString("update Storage_info "
                                    "set remain=0 "
                                    "where storageID=%1").arg(stoID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            } else {
                // 该仓库空间大于商品数量
                // 修改仓库空间信息
                modify.exec(QString("update Storage_info "
                                    "set remain=%1 "
                                    "where storageID=%2").arg(
                                    QString::number(remain - dec), stoID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            }
            // 检查该仓库中是否已有同种商品
            modify.exec(QString("select * from Storage_product "
                                "where storageID=%1 and productID=%2").arg(stoID, productID));
            if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            // 若该仓库中已有同种商品，则直接修改相应记录
            int temp = remain < dec ? remain : dec;
            if (modify.next()) {
                modify.exec(QString("update Storage_product "
                                    "set amount=amount+%1 "
                                    "where storageID=%2 and productID=%3").arg(
                                    QString::number(temp), stoID, productID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            } else {
                qDebug() << "test";
                // 若该仓库中无同种商品，则添加记录
                modify.exec(QString("insert into Storage_product "
                                    "values(%1,%2,%3)").arg(
                                    stoID, productID, QString::number(temp)));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            }
            dec -= remain;
        }
        // 修改临时占据空间变量
        StorageManage::tempOccupied -= amount;
        qDebug() << "tempOccupied:" << StorageManage::tempOccupied;
        ui->label_prompt_2->setText("提示：出库成功!");
    }
}
