#include "sys_sqlite.h"
#include "globaldata.h"

void createConnectSqlite()
{
    // 查看数据库中可用的驱动
    qDebug()<<"可用的驱动：";
    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers) {
        qDebug()<<driver;
    }

    // 创建连接，使用默认的SqLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    qDebug()<<db.databaseName();
    bool ok = db.open();
    if(ok)
        qDebug()<<"数据库连接成功";
    else
        qDebug()<<"数据库连接失败";

    // 创建数据库表
    // 表名
    QString userTable = "Sys_User";
    QString staffTable = "Sys_Staff";
    QString sellerTable = "Sys_Seller";
    QString saleOrderTable = "Sale_Order";
    QString saleStateTable = "Sale_State";
    QString storageInfoTable = "Storage_info";
    QString storageProductTable = "Storage_product";
    QString storageOrderRecordTable = "Storage_order_record";
    QString mainView = "Main_view";
    QString stockPlanTable = "stock_plan";
    QString stockProviderTable = "stock_provider";
    QString stockProviderProductTable = "stock_provider_product";
    QString stockPlanDetailTable = "stock_plan_detail";
    QString stockCanceledplanTable = "stock_canceledplan";
    QString stockCanceledplanDetailTable = "stock_canceledplan_detail";

    // 建表语句
    QString createUserSql = "create table Sys_User"
                            "("
                            "User_Id integer primary key, "
                            "User_Name varchar(20), "
                            "User_Pwd varchar(20), "
                            "User_Email varchar(20), "
                            "User_Is_Admin int"
                            ")";

    QString createStaffSql = "create table Sys_Staff"
                             "("
                             "Staff_Id varchar(10) primary key, "
                             "Staff_Name varchar(20), "
                             "Staff_Deperment varchar(20), "
                             "Staff_Position varchar(20), "
                             "Staff_Status varchar(20), "
                             "Staff_Phone varchar(20), "
                             "Staff_Date varchar(30), "
                             "Staff_Image varchar(50), "
                             "Staff_Description varchar(50)"
                             ")";

    QString createSellerSql = "create table Sys_Seller"
                              "("
                              "Seller_Id integer primary key, "
                              "Seller_Name varchar(20), "
                              "Seller_Phone varchar(20), "
                              "Seller_Addr varchar(30)"
                              ")";

    QString createSaleOrderSql = "create table Sale_Order("
                                 "Sale_Order_ID varchar(30) primary key,"
                                 "Sale_Buyer_Name varchar(20),"
                                 "Sale_Buyer_Tel varchar(20),"
                                 "Sale_Buyer_Address varchar(40),"
                                 "Sale_Seller_ID int," //外键
                                 "Sale_Item_ID int,"//外键
                                 "Sale_Item_Num int,"
                                 "Sale_Item_Price float,"
                                 "Sale_Order_Finished int)";

    QString createSaleStateSql = "create table Sale_State("
                                 "Sale_State_ID integer primary key autoincrement,"
                                 "Sale_Order_ID varchar(30),"//外键
                                 "Sale_Order_State varchar(20),"
                                 "Sale_Date datetime,"
                                 "foreign key (Sale_Order_ID) "
                                 "references Sale_Order(Sale_Order_ID) on delete cascade)";
    QString forignSql = "PRAGMA foreign_keys = ON";

    // 仓库信息表
    static QString createStorageInfoSql = "create table Storage_info("
                                  "storageID integer,"
                                  "storageName varchar(10),"
                                  "capacity integer,"
                                  "remain integer,"
                                  "sellerID integer,"
                                  "primary key(storageID),"
                                  "foreign key(sellerID) references Sys_Seller(Seller_Id)"
                                  ")";
    // 库存商品信息表
    static QString createStorageProductSql = "create table Storage_product("
                                     "storageID integer,"
                                     "productID integer,"
                                     "amount integer,"
                                     "primary key(storageID,productID),"
                                     "foreign key(storageID) references Storage_info(storageID),"
                                     "foreign key(productID) references stock_provider_product(id)"
                                     ")";

    // 出库记录表
    static QString createOrderRecordSql = "create table Storage_order_record("
                                  "orderID integer,"
                                  "productID integer,"
                                  "orderDate datetime,"
                                  "amount integer,"
                                  "primary key(orderID),"
                                  "foreign key(productID) references stock_provider_product(id)"
                                  ")";

    /*
    // 卖家信息表
    static QString seller = "create table Sys_Seller("
                            "Seller_Id integer,"
                            "Seller_Name varchar(10),"
                            "primary key(Seller_Id)"
                            ")";
    // 商品信息表
    static QString product = "create table stock_provider_product("
                             "id integer,"
                             "name integer,"
                             "price float,"
                             "primary key(id)"
                             ")";
    */

    // 界面显示view
    static QString mainViewSql = "create view Main_view as "
                               "select si.storageName, s.Seller_Name, p.name, sp.amount "
                               "from Sys_Seller s, stock_provider_product p, Storage_info si, Storage_product sp "
                               "where si.sellerID = s.Seller_Id "
                               "and sp.storageID = si.storageID "
                               "and sp.productID = p.id";

    QString stockPlanSql = "create table stock_plan("
                           "id int primary key, "
                           "seller_id int, "
                           "money float, "
                           "fee float, "
                           "time Date)";

    QString stockProviderSql = "create table stock_provider("
                               "id int primary key, "
                               "owner_id int, "
                               "name varchar, "
                               "address varchar)";

    QString stockProviderProductSql = "create table stock_provider_product("
                                      "id int primary key, "
                                      "provider_id int, "
                                      "name varchar, "
                                      "price float)";

    QString stockPlanDetailSql = "create table stock_plan_detail("
                                 "id int primary key, "
                                 "plan_id int, "
                                 "product_id int, "
                                 "cnt int, "
                                 "price float, "
                                 "state varchar)";

    QString stockCanceledplanSql = "create table stock_canceledplan("
                                   "id int primary key, "
                                   "seller_id int, "
                                   "money float, "
                                   "time Date)";

    QString stockCanceledplanDetailSql = "create table stock_canceledplan_detail("
                                         "id int primary key, "
                                         "plan_id int, "
                                         "product_id int, "
                                         "cnt int, "
                                         "price float)";
//    qDebug()<<createSaleStateSql;
    createTable(createUserSql, userTable);
    createTable(createStaffSql, staffTable);
    createTable(createSellerSql, sellerTable);
    createTable(createSaleOrderSql, saleOrderTable);
    createTable(createSaleStateSql, saleStateTable);
    Sale_Sql(forignSql);
    createTable(createStorageInfoSql, storageInfoTable);
    createTable(createStorageProductSql, storageProductTable);
    createTable(createOrderRecordSql, storageOrderRecordTable);
    createTable(mainViewSql, mainView);
    createTable(stockPlanSql, stockPlanTable);
    createTable(stockProviderProductSql, stockProviderProductTable);
    createTable(stockPlanDetailSql, stockPlanDetailTable);
    createTable(stockCanceledplanDetailSql, stockCanceledplanDetailTable);
    createTable(stockCanceledplanSql, stockCanceledplanTable);
    createTable(stockProviderSql, stockProviderTable);

    //    db.close();
}

//执行sql语句
static int Sale_Sql(const QString&sql){
    QSqlQuery query;
    query.exec(sql);
    //qDebug()<<query.lastError();
    return query.lastError().number();
}

/*
 * 创建数据库表的函数
 *
*/
void createTable(QString createSql, QString tableName)
{
    QSqlQuery query;
    //    qDebug()<<createSql;
    query.exec(createSql);
    if(isTableExist(tableName))
    {
        qDebug()<< tableName+"表已经存在，无须重新创建！";
    }
    else
    {
        if(!query.exec(createSql))
        {
            qDebug()<<"创建"+ tableName +"表失败！";
            qDebug()<<query.lastError();
        }
        else
        {
            qDebug()<<"创建"+ tableName +"表成功！";
        }
    }

}

/*
 * 判断要创建的数据库表是否存在
 *
 *
*/
bool isTableExist(QString tableName)
{
    QSqlDatabase database;
    QSqlQuery query(database);
    bool isTableExist = query.exec(
                QString(
                    "select count(*) from sqlite_master "
                    "where type='table' and name='%1'").arg(tableName));
//    qDebug()<<isTableExist;
    return isTableExist;
}

/*
 *
 * 判断用户是否存在
 *
 * */
bool userCheck(QString inputText, int num)
{
    bool exists;

    QSqlQuery query;
    QString checkName;
    switch(num)
    {
    case SYS_USER_NUMBER:
        checkName = QString("select ifnull("
                            "(select User_Name "
                            "from Sys_User "
                            "where User_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(inputText);
        break;
    case SYS_STAFF_NUMBER:
        checkName = QString("select ifnull("
                            "(select Staff_Name "
                            "from Sys_Staff "
                            "where Staff_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(inputText);
        break;
    case SYS_SELLER_NUMCER:
        checkName = QString("select ifnull("
                            "(select Seller_Name "
                            "from Sys_Seller "
                            "where Seller_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(inputText);
        break;
    default:
        break;
    }

    query.exec(checkName);

    while(query.next())
    {
        if("0" == query.value(0).toString())
            exists = true;
        else
            exists = false;
    }
    return exists;
}

/*
 *
 * 判断用户密码是否正确
 *
 * */
bool pwdCheck(QString name, QString inputText)
{
    bool flag;

    QSqlQuery query;
    QString checkName = QString("(select User_Pwd "
                                "from Sys_User "
                                "where User_Name = '%1' "
                                "limit 1)")
            .arg(name);
    query.exec(checkName);

    while(query.next())
    {
        qDebug()<<query.value(0).toString();
        if(inputText == query.value(0).toString())
            flag = true;
        else
            flag = false;
    }
    return flag;
}


/*
 *
 * 判断用户身份是否正确
 *
 * */
bool identityCheck(QString name, int inputText)
{
    bool flag;

    QSqlQuery query;
    QString checkName = QString("select User_Is_Admin "
                                "from Sys_User "
                                "where User_Name = '%1' "
                                "limit 1")
            .arg(name);
    query.exec(checkName);

    while(query.next())
    {
        if(inputText == query.value(0).toInt())
            flag = true;
        else
            flag = false;
    }
    return flag;
}

/*
 *
 * 由于卖家身份和用户应该是一样的
 * 一个卖家肯定有一个相应的用户
 * 此处只是模拟系统，不用检测是否
 * 卖家存在，因为一定存在
 * */
void recordSeller(QString input)
{
    QSqlQuery query;
    QString recordSqlSeller = QString("select * "
                                      "from Sys_Seller "
                                      "where Seller_Name = '%1' "
                                      "limit 1")
            .arg(input);
    query.exec(recordSqlSeller);

    while(query.next())
    {
        User::id = query.value(0).toInt();
        User::name = query.value(1).toString();
        User::phone = query.value(2).toString();
        User::addr = query.value(3).toString();
    }
}

/*  数据库操作函数
 *
 *
 * */
void sqlOperator(QString sql)
{
    QSqlQuery query;
    query.exec(sql);
}


/*
 *
 * 根据查询条件来判断职工是否存在
 *
 * */
bool checkExist(QString input, int number)
{
    bool flag;

    QSqlQuery query;
    QString checkSql;

    switch (number) {
    case STAFF_ID_NUMBER:
        checkSql = QString("select ifnull("
                            "(select Staff_Id "
                            "from Sys_Staff "
                            "where Staff_Id = '%1' "
                            "limit 1), "
                            "0)")
                .arg(input);
        break;
    case STAFF_DEPENTMENT_NUMBER:
        checkSql = QString("select ifnull("
                            "(select Staff_Deperment "
                            "from Sys_Staff "
                            "where Staff_Deperment = '%1' "
                            "limit 1), "
                            "0)")
                .arg(input);
        break;
    case STAFF_NAME_NUMBER:
        checkSql = QString("select ifnull("
                            "(select Staff_Name "
                            "from Sys_Staff "
                            "where Staff_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(input);
        break;
    default:
        break;
    }

    query.exec(checkSql);

    while(query.next())
    {
        if(input == query.value(0).toString())
            flag = true;
        else
            flag = false;
    }
    return flag;
}
