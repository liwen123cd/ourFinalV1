#ifndef STORAGEDIALOG_H
#define STORAGEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include "mysqlquerymodel.h"

class QSqlQueryModel;
class QueryDialog;
class InsertDialog;

namespace Ui {
class StorageDialog;
}

class StorageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StorageDialog(QWidget *parent = 0);
    ~StorageDialog();

private slots:
    void on_comboBox_sto_activated(const QString &arg1);

    void on_pushBtn_query_all_clicked();

    void on_pushBtn_query_clicked();

    void getQuery(QString option, QString content);

    void getInsert(QString stoName, QString proName, QString amount);

    void on_pushBtn_insert_clicked();

    void on_pushBtn_del_clicked();

    void on_comboBox_op_activated(const QString &arg1);

    void on_pushBtn_confirm_clicked();

private:
    Ui::StorageDialog *ui;
    MySqlQueryModel *model;
    QSqlDatabase db;
    QueryDialog *queryDialog;
    InsertDialog *insertDialog;
};

#endif // STORAGEDIALOG_H
