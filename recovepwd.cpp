#include "recovepwd.h"
#include "ui_recovepwd.h"

RecovePwd::RecovePwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecovePwd)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel(0, 1, this);
    QCompleter *m_completer = new QCompleter(m_model, this);
    ui->lineEdit->setCompleter(m_completer);

    connect(m_completer, SIGNAL(activated(const QString&)), this, SLOT(onEmailChoosed(const QString&)));
    connect(ui->lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onTextChanged(const QString&)));
}

RecovePwd::~RecovePwd()
{
    delete ui;
}

void RecovePwd::on_pushButton_clicked()
{
    QString email = ui->lineEdit->text();
    if(email.length() == 0)
    {
        QMessageBox::information(this, "提示", "未填写邮箱！");
    }
    else
    {
        QMessageBox::information(this, "提示", "发送成功！"
                                             "请按提示步骤找回密码！");
    }
}

void RecovePwd::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
}

void RecovePwd::onEmailChoosed(const QString& email)
{
    ui->lineEdit->clear();    // 清除已存在的文本更新内容
    ui->lineEdit->setText(email);
}

void RecovePwd::onTextChanged(const QString& str)
{
    if (str.contains("@"))   // 如果已经输入了@符号，我们就停止补全了。因为到了这一步，我们再补全意义也不大了。
    {
        return;
    }
    QStringList strlist;
    strlist << "@163.com" << "@qq.com" << "@gmail.com" << "@hotmail.com" << "@126.com";

    m_model->removeRows(0, m_model->rowCount());   // 先清楚已经存在的数据，不然的话每次文本变更都会插入数据，最后出现重复数据
    for (int i = 0; i < strlist.size(); ++i)
    {
        m_model->insertRow(0);
        m_model->setData(m_model->index(0, 0), str + strlist.at(i));
    }
}
