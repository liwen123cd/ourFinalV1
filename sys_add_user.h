#ifndef SYS_ADD_USER_H
#define SYS_ADD_USER_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class sys_add_user;
}

class sys_add_user : public QDialog
{
    Q_OBJECT

public:
    explicit sys_add_user(QDialog *parent = 0);
    ~sys_add_user();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::sys_add_user *ui;
};

#endif // SYS_ADD_USER_H
