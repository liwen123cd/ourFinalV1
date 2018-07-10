#ifndef SALE_DETAIL_DIALOG_H
#define SALE_DETAIL_DIALOG_H

#include <QDialog>

class QSqlTableModel;
struct Sale_Order_Detail;

namespace Ui {
class Sale_Detail_Dialog;
}

class Sale_Detail_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Sale_Detail_Dialog(QWidget *parent = 0);
    ~Sale_Detail_Dialog();


private slots:
    void on_Sale_enter_pushbutton_clicked();

    void on_Sale_cancel_pushbutton_clicked();

    void Sale_Recive_Detail(const Sale_Order_Detail&);

    void on_Sale_item_id_combobox_currentIndexChanged(const QString &arg1);

signals:
    void Sale_Send_Detail(const Sale_Order_Detail&);
    void Sale_Cancel(const int);
private:
    Ui::Sale_Detail_Dialog *ui;
    QString Order_ID;
    int Row;
    QString Order_State;
    QSqlTableModel *Sale_Table_Model;
    //根据订单号读订单状态表
    bool Sale_Show_State(const QString&);
    //更改界面显示模式（可读，可写）
    bool Sale_Model_Change(const QString&);
    //显示商品详细信息
    bool Sale_Show_Item(const QString&);
    //监测订单信息是否正确
    bool Sale_Check_Detail();
    //是否是查看
    void Sale_Is_Check(bool);
};

#endif // SALE_DETAIL_DIALOG_H
