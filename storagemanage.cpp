#include "storagemanage.h"
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>
#include <QStringList>

static int userID = 1;    // 全局用户ID
QSqlDatabase StorageManage::db = QSqlDatabase::database();
int StorageManage::tempOccupied = 0;

StorageManage::StorageManage()
{

}

StorageManage::~StorageManage()
{

}

// 获取卖家仓库剩余空间
int StorageManage::restSpace()
{
    int rest = 0;
    QSqlQuery query(db);
    query.exec(QString("select sum(remain) from Storage_info "
                       "where sellerID=%1").arg(QString::number(userID)));
    if (query.next()) {
        rest = query.value(0).toInt();
    }
    rest -= tempOccupied;
    qDebug() << "卖家剩余空间" << rest << endl;
    return rest;
}

// 销售、出库
int StorageManage::sellOut(int orderID, int productID, int num)
{
    int restPro;
    QSqlQuery query(db);

    // 检查商品ID是否存在
    query.exec(QString("select * from Storage_product where productID=%1").arg(productID));
    if (!query.next()) {
        qDebug() << "商品ID不存在";
        return 1;
    }

    // 检查商品余量是否足够
    query.exec(QString("select sum(sp.amount) from Storage_product sp, Storage_info si "
                       "where sp.storageID=si.storageID "
                       "and si.sellerID=%1 "
                       "and sp.productID=%2").arg(QString::number(userID), QString::number(productID)));
    if (query.next()) {
        restPro = query.value(0).toInt();
        if (restPro < num) {
            qDebug() << "余量不足";
            return 2;
        }
    }

    // 修改数据库
    // dec用于保存每个仓库出库后的剩余所需值
    int dec = num;
    query.exec(QString("select sp.storageID, sp.productID from Storage_product sp, Storage_info si "
                       "where sp.storageID=si.storageID "
                       "and si.sellerID=%1 "
                       "and sp.productID=%2").arg(QString::number(userID), QString::number(productID)));
    // 对于每一条记录
    while (dec > 0 && query.next()) {
        QString stoID = query.value(0).toString();
        QString proID = query.value(1).toString();
        int curAmount = 0;
        QSqlQuery modify(db);
        modify.exec(QString("select amount from Storage_product "
                            "where storageID=%1 and productID=%2").arg(stoID, proID));
        if (modify.next()) curAmount = modify.value(0).toInt();
        // 若该记录中商品数量不大于dec，则删除该条记录
        if (curAmount <= dec) {
            modify.exec(QString("delete from Storage_product "
                                "where storageID=%1 and producrID=%2").arg(stoID, proID));
            if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
        } else {
            // 否则修改该条记录的剩余数量
            modify.exec(QString("update Storage_product "
                                "set amount=%1 "
                                "where storageID=%2 and productID=%3").arg(
                                QString::number(curAmount - dec), stoID, proID));
            if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
        }
        // 修改仓库剩余空间值
        modify.exec(QString("update Storage_info "
                            "set remain=%1"
                            "where storageID=%2").arg(
                            QString::number(min(curAmount, dec)), stoID));
        if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
        dec -= curAmount;
    }
    qDebug() << dec;

    // 向出库记录表中增添一条记录
    QDateTime curTime = QDateTime::currentDateTime();
    query.exec(QString("insert into Storage_order_record "
                       "values(%1,%2,%3,%4)").arg(
                       QString::number(orderID),
                       QString::number(productID),
                       curTime.toString(),
                       QString::number(num)));
    if (query.lastError().isValid()) qDebug() << query.lastError().text();
    return 0;
}

// 为新添卖家分配仓库
int StorageManage::allocateStorage(int sellerID, int storageNum)
{
    qDebug() << "分配仓库";
    int restStoNum = 0;
    QSqlQuery query(db);

    // 查看剩余空闲仓库数量
    query.exec(QString("select count(*) from Storage_info "
                       "where sellerID=-1"));
    if (query.next()) {
        restStoNum = query.value(0).toInt();
        qDebug() << "空闲仓库数量：" << restStoNum;
    }

    // 不足则返回1
    if (restStoNum < storageNum) {
        qDebug() << "仓库不足";
        return 1;
    }

    // 进行分配
    int i = storageNum;
    QSqlQuery modify(db);
    query.exec(QString("select storageID from Storage_info "
                       "where sellerID=-1"));
    while(i > 0 && query.next()) {
        QString stoID = query.value(0).toString();
        modify.exec(QString("update Storage_info "
                            "set sellerID=%1 "
                            "where storageID=%2").arg(
                            QString::number(sellerID), stoID));
        if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
        --i;
    }
    qDebug() << "分配完成" << endl;
    return 0;
}

// 回收已删除卖家的仓库
int StorageManage::freeStorage(int sellerID)
{
    qDebug() << "回收仓库";
    QSqlQuery query(db);
    // 检查所给卖家ID是否存在
    query.exec(QString("select * from Storage_info "
                       "where sellerID=%1").arg(
                       QString::number(sellerID)));
    if (!query.next()) {
        qDebug() << "卖家ID不存在";
        return 1;
    }
    // 检查卖家仓库内是否还有商品
    query.exec(QString("select * from Storage_info si, Storage_product sp "
                       "where sp.storageID=si.storageID "
                       "and si.sellerID=%1").arg(QString::number(sellerID)));
    if (query.lastError().isValid()) qDebug() << query.lastError().text();
    if (query.next()) {
        int remain = query.value(0).toInt();
        if (remain > 0) {
            qDebug() << "该卖家仓库未清空";
            return 2;
        }
    }
    // 将仓库拥有者设置为空
    query.exec(QString("update Storage_info "
                       "set sellerID=-1 "
                       "where sellerID=%1").arg(
                       QString::number(sellerID)));
    if (query.lastError().isValid()) qDebug() << query.lastError().text();
    qDebug() << "回收完成" << endl;
    return 0;
}

// 获取出库记录中某段时间内某商品的总数量
int StorageManage::getRecordNum(QDateTime startTime, QDateTime endTime, int productID = -1)
{
    int num;
    QSqlQuery query(db);
    if (-1 == productID) {
        // 查询所有商品数量
        query.exec(QString("select sum(amount) from Storage_order_record "
                           "where orderDate between '%1' and '%2'").arg(
                           startTime.toString(), endTime.toString()));
    } else {
        // 查询指定商品数量
        query.exec(QString("select sum(amount) from Storage_order_record "
                           "where productID=%1 "
                           "and orderDate between '%2' and '%3'").arg(
                           QString::number(productID), startTime.toString(), endTime.toString()));
    }
    if (!query.next()) {
        qDebug() << "查询出错";
        return -1;
    }
    num = query.value(0).toInt();
    qDebug() << "数量为" << num;
    return num;
}

// 修改仓库剩余库存，实际上是修改tempOccupied
int StorageManage::changeRemainSpace(int num)
{
    qDebug() << "修改仓库剩余空间";
    tempOccupied += num;
    qDebug() << "tempOccupied: " << tempOccupied;
    return 0;
}

// 获取商品列表
QStringList StorageManage::getProductList()
{
    QStringList proList;
    QSqlQuery query(db);
    query.exec(QString("select p.name "
                       "from Storage_product sp, Storage_info si, stock_provider_product p "
                       "where sp.storageID=si.storageID "
                       "and sp.productID=p.id "
                       "and si.sellerID=%1").arg(QString::number(userID)));
    if (query.lastError().isValid()) qDebug() << query.lastError().text();
    while (query.next()) {
        if (!proList.contains(query.value(0).toString()))
            proList << query.value(0).toString();
    }
    return proList;
}

// 获取商品库存数量
int StorageManage::getAmount(int productID)
{
    int amount = 0;
    QSqlQuery query(db);
    query.exec(QString("select sum(amount) "
                       "from Storage_product sp, Storage_info si "
                       "where sp.storageID=si.storageID "
                       "and si.sellerID=%1 "
                       "and sp.productID=%2").arg(
                       QString::number(userID), QString::number(productID)));
    if (query.next()) {
        amount = query.value(0).toInt();
    }
    return amount;
}

int StorageManage::min(int a, int b)
{
    if (a <= b) {
        return a;
    } else {
        return b;
    }
}
