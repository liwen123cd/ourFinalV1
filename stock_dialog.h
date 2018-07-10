#ifndef STOCK_DIALOG_H
#define STOCK_DIALOG_H

#include <QDialog>

namespace Ui {
class stock_Dialog;
}

class stock_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit stock_Dialog(QWidget *parent = 0);
    ~stock_Dialog();

    void set_content(QString product_name, float price, QString provider_name, QString address);
private:
    Ui::stock_Dialog *ui;
};

#endif // STOCK_DIALOG_H
