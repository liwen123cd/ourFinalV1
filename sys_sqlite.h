#ifndef SYS_SQLITE_H
#define SYS_SQLITE_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>

#define SYS_USER_NUMBER 1
#define SYS_STAFF_NUMBER 2
#define SYS_SELLER_NUMCER 3
#define STAFF_ID_NUMBER 1
#define STAFF_DEPENTMENT_NUMBER 2
#define STAFF_NAME_NUMBER 3
#define STAFF_ID "员工ID"
#define STAFF_DEPENTMENT "部门"
#define STAFF_NAME "姓名"

void createConnectSqlite();
bool isTableExist(QString tableName);
void createTable(QString createSql, QString tableName);
bool userCheck(QString inputText, int num);
bool pwdCheck(QString name, QString inputText);
bool identityCheck(QString name, int inputText);
void recordSeller(QString input);
void sqlOperator(QString sql);
bool checkExist(QString input, int number);
static int Sale_Sql(const QString&sql);

#endif // SYS_SQLITE_H
