#ifndef GLOBALDATA
#define GLOBALDATA
#include <QString>
#include <QDateTime>

class Data
{
public:
    static int is_admin;
};

class User
{
public:
    static int id;
    static QString name;
    static QString phone;
    static QString addr;
};

class Staff
{
public:
    static QString id;
    static QString name;
    static QString deperment;
    static QString position;
    static QString status;
    static QString phone;
    static QString date;
    static QString image;
    static QString description;
};

struct Sale_Order_Detail
{
    //订单信息
    int Sale_Row;
    QString Sale_Order_ID;
    QString Sale_Buyer_Name;
    QString Sale_Buyer_Tel;
    QString Sale_Buyer_Address;
    int Sale_Item_ID;
    int Sale_Item_Num;
    float Sale_Item_Price;//单价
    QString Sale_State;

};

struct Sale_State_Detail
{
    QString Sale_Order_ID;
    QString Sale_Order_State;
    QDateTime Sale_Date;
};

struct Product_Detail
{
    //商品信息
    QString Product_Name;
    float Product_Price;
    QString Product_Provider;

};




#endif // GLOBALDATA

