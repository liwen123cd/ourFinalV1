#ifndef GLOBALDATA
#define GLOBALDATA
#include <QString>

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

#endif // GLOBALDATA

