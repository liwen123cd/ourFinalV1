#ifndef INSERTDIALOG_H
#define INSERTDIALOG_H

#include <QDialog>

class QString;

namespace Ui {
class InsertDialog;
}

class InsertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertDialog(QWidget *parent = 0);
    ~InsertDialog();

signals:
    void sendMsg(QString stoName, QString proName, QString amount);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::InsertDialog *ui;
};

#endif // INSERTDIALOG_H
