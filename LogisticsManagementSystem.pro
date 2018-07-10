#-------------------------------------------------
#
# Project created by QtCreator 2018-06-23T18:38:50
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogisticsManagementSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    register.cpp \
    usermanager.cpp \
    staffmanager.cpp \
    recovepwd.cpp \
    mainwindow.cpp \
    sys_add_user.cpp \
    sys_add_staff.cpp \
    sys_sqlite.cpp \
    globaldata.cpp \
    login.cpp \
    sys_maintain_staff.cpp \
    sale_widget.cpp \
    sale_detail_dialog.cpp \
    sale_stocktaking_dialog.cpp \
    stock_provider.cpp \
    stock_mkplan.cpp \
    stock_srplan.cpp \
    stock_dialog.cpp \
    stock_mainwindow.cpp \
    storagedialog.cpp \
    querydialog.cpp \
    mysqlquerymodel.cpp \
    insertdialog.cpp \
    storagemanage.cpp \
    myitemdelegate.cpp

HEADERS += \
    register.h \
    usermanager.h \
    staffmanager.h \
    recovepwd.h \
    mainwindow.h \
    sys_add_user.h \
    sys_add_staff.h \
    globaldata.h \
    login.h \
    sys_sqlite.h \
    sys_maintain_staff.h \
    sale_widget.h \
    sale_detail_dialog.h \
    sale_stocktaking_dialog.h \
    stock_dialog.h \
    stock_mainwindow.h \
    storagedialog.h \
    querydialog.h \
    mysqlquerymodel.h \
    insertdialog.h \
    storagemanage.h \
    myitemdelegate.h

FORMS += \
        login.ui \
    register.ui \
    usermanager.ui \
    staffmanager.ui \
    recovepwd.ui \
    mainwindow.ui \
    sys_add_user.ui \
    sys_add_staff.ui \
    sys_maintain_staff.ui \
    sale_widget.ui \
    sale_detail_dialog.ui \
    sale_stocktaking_dialog.ui \
    stock_dialog.ui \
    stock_mainwindow.ui \
    storagedialog.ui \
    querydialog.ui \
    insertdialog.ui

RESOURCES += \
    staff_img.qrc \
    style.qrc

DISTFILES += \
    LogisticsManagementSystem.pro.user \
    number.txt

QMAKE_CXXFLAGS += -std=c++11
