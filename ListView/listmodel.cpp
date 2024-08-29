#include "listmodel.h"

MyListModel::MyListModel(QObject *parent)
    : QObject{parent}
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

void MyListModel::setItems(const QVariantList &newItems)
{
    if (items == newItems)
        return;
    items = newItems;
    emit itemsChanged();
}
