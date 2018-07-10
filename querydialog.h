#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>

class QString;

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = 0);
    ~QueryDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void sendMsg(QString option, QString content);

private:
    Ui::QueryDialog *ui;
};

#endif // QUERYDIALOG_H
