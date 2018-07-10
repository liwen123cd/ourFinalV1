#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QCloseEvent>

#include "login.h"


namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

    bool textCheck();
//    bool userCheck();
public slots:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
