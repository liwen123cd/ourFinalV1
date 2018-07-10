#ifndef MYSQLQUERYMODEL_H
#define MYSQLQUERYMODEL_H

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QStringList>

class QStringList;

class MySqlQueryModel : public QSqlQueryModel
{
public:
    MySqlQueryModel();
    ~MySqlQueryModel();
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);  
    QStringList getStorageNames() const;

private:
    bool setOwnStorage(int storageID, int productID, int newStorageID);
    bool setAmount(int storageID, int productID, int newAmount);
    int getStorageID(QString storageName);
    int getProductID(QString productName);
    void refresh();

    QSqlDatabase db;
    QStringList *cache;
};

#endif // MYSQLQUERYMODEL_H
