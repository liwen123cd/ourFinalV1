#include "login.h"
#include "register.h"
#include "usermanager.h"
#include "staffmanager.h"
#include "sys_sqlite.h"
#include "mainwindow.h"
#include "staffmanager.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//正确显示中文
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//正确显示中文

    createConnectSqlite();


    Login login;
    login.show();

//    UserManager um;
//    um.show();

//    StaffManager sm;
//    sm.show();

    return a.exec();
}
