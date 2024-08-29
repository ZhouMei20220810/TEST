#include "listmodel.h"

ListModel::ListModel(QObject *parent)
    : QObject{parent}
{}

ListModel *ListModel::getInstance()
{
    static ListModel* listmodel = new ListModel();
    return listmodel;
}

QVariantList ListModel::getItems() const
{
    return items;
}

void ListModel::setItems(const QVariantList &newItems)
{
    if (items == newItems)
        return;
    items = newItems;
    emit itemsChanged();
}
