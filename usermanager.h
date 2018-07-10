#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>

class QSqlTableModel;
namespace Ui {
class UserManager;
}

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = 0);
    ~UserManager();

    void sqLite();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::UserManager *ui;
    QSqlTableModel *model;
    void tableModel();
};

#endif // USERMANAGER_H
