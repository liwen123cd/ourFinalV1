#include "mysqlquerymodel.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QString>
#include <QDebug>

MySqlQueryModel::MySqlQueryModel()
{
    db = QSqlDatabase::database();
}

MySqlQueryModel::~MySqlQueryModel()
{

}

// 重写flags方法
Qt::ItemFlags MySqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 0 || index.column() == 3) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

// 重写setData方法
bool MySqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() == 1 || index.column() == 2) {
        return false;
    }
    // 获取仓库ID，商品ID
    int storageID, productID;
    QString storageName = data(QSqlQueryModel::index(index.row(), 0)).toString();
    QString productName = data(QSqlQueryModel::index(index.row(), 2)).toString();
    storageID = getStorageID(storageName);
    productID = getProductID(productName);
    //qDebug() << storageID << productID;
    if (storageID < 0 || productID < 0) {
        return false;
    }
    // 修改
    bool ok = false;
    if (index.column() == 0) {
        int newStorageID = getStorageID(value.toString());
        if (newStorageID < 0) {
            return false;
        }
        ok = setOwnStorage(storageID, productID, newStorageID);
    } else if (index.column() == 3) {
        ok = setAmount(storageID, productID, value.toInt());
    }
    refresh();
    return ok;
}

// 修改商品所属仓库
bool MySqlQueryModel::setOwnStorage(int storageID, int productID, int newStorageID)
{
    QSqlQuery query(db);
    query.prepare("update Storage_product set storageID=? "
                  "where storageID=? and productID=?");
    query.addBindValue(newStorageID);
    query.addBindValue(storageID);
    query.addBindValue(productID);
    return query.exec();
}

// 修改数量
bool MySqlQueryModel::setAmount(int storageID, int productID, int newAmount)
{
    QSqlQuery query(db);
    query.prepare("update Storage_product set amount=? "
                  "where storageID=? and productID=?");
    query.addBindValue(newAmount);
    query.addBindValue(storageID);
    query.addBindValue(productID);
    return query.exec();
}

// 根据仓库名获取仓库ID
int MySqlQueryModel::getStorageID(QString storageName)
{
    QSqlQuery query(db);
    query.exec(QString("select storageID from Storage_info "
                       "where storageName='%1'").arg(storageName));
    if (!query.next()) {
        return -1;
    }
    return query.value(0).toInt();
}

// 根据物品名获取物品ID
int MySqlQueryModel::getProductID(QString productName)
{
    QSqlQuery query(db);
    query.exec(QString("select productID from Product "
                       "where productName='%1'").arg(productName));
    if (!query.next()) {
        return -1;
    }
    return query.value(0).toInt();
}

// 刷新
void MySqlQueryModel::refresh()
{
    setQuery("select * from Main_view", db);
    setHeaderData(0, Qt::Horizontal, "仓库");
    setHeaderData(1, Qt::Horizontal, "卖家");
    setHeaderData(2, Qt::Horizontal, "商品");
    setHeaderData(3, Qt::Horizontal, "数量");
}

// 获取仓库名
QStringList MySqlQueryModel::getStorageNames() const
{
    QStringList nameList;
    QSqlQuery query(db);
    query.exec(QString("select storageName from Storage_info"));
    while (query.next()) {
        nameList << query.value(0).toString();
    }
    return nameList;
}
