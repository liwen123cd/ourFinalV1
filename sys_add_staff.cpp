#include "sys_add_staff.h"
#include "ui_sys_add_staff.h"
#include "sys_sqlite.h"

Sys_Add_Staff::Sys_Add_Staff(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Add_Staff)
{
    ui->setupUi(this);
    number = 0;
    staff_filename = "D://Documents/QtProject/LogisticsManagementSystem/img/na.jpg";
//    qDebug()<<QDir::currentPath();
//    qDebug()<<QCoreApplication::applicationDirPath();
    //    QFile(directoryOf("data").absoluteFilePath(number.txt));
    QFile file("D://Documents/QtProject/LogisticsManagementSystem/number.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "打开文件失败." << endl;
        return ;
    }
    while(!file.atEnd())
    {
        QString line = file.readLine();
        qDebug()<< line;
        number = line.toInt();
    }
    qDebug()<<number;
    file.close();
    ui->radioButton->setChecked(true);
}

Sys_Add_Staff::~Sys_Add_Staff()
{
    delete ui;
}

/*
 *
 * 添加员工
 * 默认员工在职
 *
 * */
void Sys_Add_Staff::on_pushButton_2_clicked()
{
    QString staff_name = ui->lineEdit->text();
    QString staff_deperment;
    if(ui->radioButton->isChecked())
    {
        staff_deperment = ui->radioButton->text();
    }
    if(ui->radioButton_2->isChecked())
    {
        staff_deperment = ui->radioButton_2->text();
    }
    QString staff_position = ui->comboBox->currentText();
    QString staff_phone = ui->lineEdit_2->text();
    QString staff_description = ui->textEdit->toPlainText();
    QString staff_status = STATUS_AT;
    QString staff_date = QString(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    qDebug()<<staff_date;
    QString staff_date2 = QString(QDateTime::currentDateTime().toString("yyyyMMdd"));
    qDebug()<<staff_date2;
    QString staff_id = NULL;
    if(STOCK_DEPENTMENT == staff_deperment)
        staff_id = "SD" + staff_date2 + QString("%1").arg(number, 5, 10, QLatin1Char('0'));
    else if(FREIGHT_DEPENTMENT == staff_deperment)
        staff_id = "FD" + staff_date2 + QString("%1").arg(number, 5, 10, QLatin1Char('0'));
    qDebug()<<staff_id;
    number++;
    // 将number 值写入文件
    QFile file("D://Documents/QtProject/LogisticsManagementSystem/number.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "文件打开失败！" << endl;
        return ;
    }
    QTextStream out(&file);
    out << number;
    file.close();

    if(NULL == staff_name)
        QMessageBox::warning(this, tr("提示"),
                             "请填写职工姓名！");
    if(NULL == staff_phone)
        QMessageBox::warning(this, tr("提示"),
                             "请填写职工电话！");
    if(NULL != staff_name && NULL != staff_phone)
    {
        if(!userCheck(staff_name, SYS_STAFF_NUMBER))
        {
            QMessageBox::warning(this, tr("提示"),
                                 "职工已存在，请确认职工姓名！");
        }
        else
        {
            QString add_sql = QString("insert into Sys_Staff(Staff_Id, "
                                      "Staff_Name, Staff_Deperment, "
                                      "Staff_Position, Staff_Status, "
                                      "Staff_Phone, Staff_Date, "
                                      "Staff_Image, Staff_Description)");
            add_sql += QString(" VALUES('%1','%2','%3','%4',"
                               "'%5', '%6', '%7', '%8', '%9')")
                    .arg(staff_id)
                    .arg(staff_name)
                    .arg(staff_deperment)
                    .arg(staff_position)
                    .arg(staff_status)
                    .arg(staff_phone)
                    .arg(staff_date)
                    .arg(staff_filename)
                    .arg(staff_description);

            QSqlQuery query;
            query.exec(add_sql);

            QMessageBox::information(this, "添加成功",
                                     "您已成功添加新职工！");
            this->close();
        }
    }

}

void Sys_Add_Staff::on_pushButton_clicked()
{
    staff_filename = QFileDialog::getOpenFileName(this,tr("选择图片"),
                                                  "",tr("Images(*.png *.jpg *.jpeg)"));
    if (QString::compare(staff_filename, QString()) != 0)
    {
        QImage image;
        bool valid = image.load(staff_filename);
        if(valid)
        {
            image = image.scaledToWidth(ui->label_7->width(), Qt::SmoothTransformation);
            ui->label_7->setPixmap(QPixmap::fromImage(image));
        }
    }
    qDebug()<<staff_filename;
}

void Sys_Add_Staff::on_pushButton_3_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->radioButton->setChecked(true);
    ui->comboBox->setCurrentIndex(0);
    ui->textEdit->clear();
    staff_filename = ":/new/img/img/na.jpg";
    ui->label_7->setPixmap(QPixmap(staff_filename));
}
