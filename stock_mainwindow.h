#ifndef stock_MainWindow_H
#define stock_MainWindow_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <unordered_map>
#include <QTableWidgetItem>
namespace Ui {
class stock_MainWindow;
}

class stock_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    #define GLOBAL_USER  0xFFFFFFFF
    explicit stock_MainWindow(QWidget *parent = 0);
    ~stock_MainWindow();

    void set_restSpace();
public slots:

    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_pushButton_7_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void stock_tableview_3_clicked(const QModelIndex &index);
    void on_pushButton_8_clicked();
    void stock_tableview_2_clicked(const QModelIndex &index);
    void stock_tableview_5_doubleclicked(const QModelIndex &index);
    void on_pushButton_9_clicked();
    void stock_tablewidget_2_changed(QTableWidgetItem*);
    void stock_tablewidget_2_pressed(QTableWidgetItem*);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void stock_tableview_6_clicked(const QModelIndex &index);
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_11_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_clicked();
    void stock_tableview_doubleclicked(const QModelIndex &index);
private:
    Ui::stock_MainWindow *ui;
    QSqlTableModel*stock_provider;
    QSqlTableModel*stock_provider_product;
    QSqlQueryModel*stock_mkplan_provider;
    QSqlQueryModel*stock_mkplan_product;
    QSqlQueryModel*stock_srplan_basic;
    QSqlQueryModel*stock_srplan_detail;
    int user_id;
    bool is_admin;
    int  storage_space;
    std::unordered_map<int,int> choosen_id;
    void set_provider_visible(bool b);
    void stock_provider_model_init();
    void set_provider_modify(bool b);
    void stock_set_headname();
    int stock_make_id(int type);
    void stock_refresh_provider();
    void stock_mkplan_init();
    void stock_mkplan_edit(int rowNum);
    void stock_mkplan_change(const QModelIndex &index, int num);
    float stock_getProduct_price(int product_id);
    void set_price_fee();
    void stock_mkplan_create();
    void stock_srplan_init();
    void stock_srplan_init_aux(int);
    void stock_srplan_noncompleted(int);
    void stock_srplan_canceled();
    void stock_real_cancel(int rowNum);
    void stock_srplan_canceled_aux();
};

#endif // stock_MainWindow_H
