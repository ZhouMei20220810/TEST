#include "listmodel.h"

MyListModel::MyListModel(QObject* parent)
    : QObject{ parent }
{}

MyListModel* MyListModel::getInstance()
{
    static MyListModel* listmodel = new MyListModel();
    return listmodel;
}

QVariantList MyListModel::getItems() const
{
    return items;
}

void MyListModel::setItems(const QVariantList& newItems)
{
    if (items == newItems)
        return;
    items = newItems;
    emit itemsChanged();
}

void MyListModel::ShowInstanceSignalFromQMLFile(int iId, QString strPhoneName, QString strInstanceNo, QString strExpireTime, bool bIsShowMenu)
{
    //后续可以考虑是否要根据id找全S_PHONE_INFO的信息
    qDebug() << "ShowInstanceSignalFromQMLFile strPhoneName=" << strPhoneName << " VMNo=" << strInstanceNo << "iId=" << iId << "strExpireTime=" << strExpireTime << "bIsShowMenu=" << bIsShowMenu;

}

void MyListModel::do_ItemClickSignals(int index, const QVariant& data)
{
    qDebug() << "Clicked item at index:" << index;
    qDebug() << "Item data:" << data;
}

void MyListModel::do_notifyRefreshWindow()
{
    qDebug() << "slot MyListModel do_notifyRefreshWindow";
    emit notifyMainWindowRefreshWindow();
}

void MyListModel::setCancelSelectCheckBox(bool bChecked)
{
    QList<ListItem*> list;
    for (const QVariant& variant : items) {
        ListItem* item = variant.value<ListItem*>();
        item->setCheckBox(false);
        list.append(item);
    }
}

void MyListModel::setAllCheckBox(bool newCheckBox)
{
    QList<ListItem*> list;
    for (const QVariant& variant : items) {
        ListItem* item = variant.value<ListItem*>();
        item->setCheckBox(newCheckBox);
        list.append(item);
    }
}

//反选
void MyListModel::setFanXuanCheckBox()
{
    QList<ListItem*> list;
    for (const QVariant& variant : items) {
        ListItem* item = variant.value<ListItem*>();
        item->setCheckBox(!item->getCheckBox());
        list.append(item);
    }
}