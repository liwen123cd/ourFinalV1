#ifndef RECOVEPWD_H
#define RECOVEPWD_H

#include <QWidget>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QCompleter>

namespace Ui {
class RecovePwd;
}

class RecovePwd : public QWidget
{
    Q_OBJECT

public:
    explicit RecovePwd(QWidget *parent = 0);
    ~RecovePwd();

public slots:
    void onEmailChoosed(const QString &email);
    void onTextChanged(const QString &str);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::RecovePwd *ui;
    QStandardItemModel *m_model;
};

#endif // RECOVEPWD_H
