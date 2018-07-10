#ifndef STORAGEMANAGE_H
#define STORAGEMANAGE_H

#include <QSqlDatabase>
#include <QDateTime>
#include <QStringList>

class StorageManage
{
public:
    StorageManage();
    ~StorageManage();

    static int allocateStorage(int sellerID, int storageNum);     // 为新增卖家分配仓库
    static int freeStorage(int sellerID);                         // 回收已删除的卖家的仓库

    static int restSpace();                                       // 获取卖家仓库剩余空间
    static int changeRemainSpace(int num);                        // 修改库存剩余空间

    static int sellOut(int orderID, int productID, int num);      // 出库
    static QStringList getProductList();                          // 获取商家所拥有的商品的列表
    static int getAmount(int productID);                          // 根据商品编号获取库存数量
    static int getRecordNum(QDateTime startTime,                  // 获取出库记录表中相应商品的总数量
                            QDateTime endTime,
                            int productID);

    static int tempOccupied;    // 已下进货订单但尚未到达仓库的货品所占空间

private:
    static QSqlDatabase db;


    static int min(int a, int b);
};

#endif // STORAGEMANAGE_H
