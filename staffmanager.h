#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "globaldata.h"

namespace Ui {
class StaffManager;
}

class StaffManager : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManager(QWidget *parent = 0);
    ~StaffManager();

    void queryStaff(QString input);
    void init();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

private:
    Ui::StaffManager *ui;
    QSqlTableModel *model;
    Staff *staff;
};

#endif // STAFFMANAGER_H
