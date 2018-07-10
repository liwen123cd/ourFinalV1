#include "myitemdelegate.h"
#include <QComboBox>
#include <QString>
#include <QStringList>
#include "mysqlquerymodel.h"

MyItemDelegate::MyItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

MyItemDelegate::~MyItemDelegate()
{

}

// 创建编辑器
QWidget * MyItemDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    if (index.column() == 0) {
        QComboBox *combo = new QComboBox(parent);
        MySqlQueryModel *model = (MySqlQueryModel*)(index.model());
        QStringList nameList = model->getStorageNames();
        while (!nameList.empty()) {
            combo->addItem(nameList.front());
            nameList.pop_front();
        }
        return combo;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

// 设置编辑器数据
void MyItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 0) {
        QComboBox *combo = static_cast<QComboBox*>(editor);
        if (combo) {
            QString str = index.model()->data(index, Qt::EditRole).toString();
            combo->setCurrentIndex(combo->findText(str));
        }
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

// 设置model数据
void MyItemDelegate::setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
{
    if (index.column() == 0) {
        QComboBox *combo = static_cast<QComboBox*>(editor);
        model->setData(index, combo->currentText(), Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

// 设置外观
void MyItemDelegate::updateEditorGeometry(QWidget *editor,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
