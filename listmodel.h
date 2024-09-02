#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QtQml>
#include "listitem.h"

class MyListModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT //声明QML可以访问元素
public:
    explicit MyListModel(QObject *parent = nullptr);

    //单实例
    static MyListModel* getInstance();
    QVariantList getItems() const;
    void setItems(const QVariantList &newItems);

    QList<ListItem *> itemList()
    {
        QList<ListItem *> list;
        for (const QVariant &variant : items) {
            ListItem *item = variant.value<ListItem *>();
            list.append(item);
        }
        return list;
    }

    void addItem(ListItem* item)
    {
        items.append(QVariant::fromValue(item));
        emit itemsChanged();
    }

    void removeItem(ListItem *item)
    {
        items.removeOne(QVariant::fromValue(item));
        emit itemsChanged();
    }

    //移除所有项
    void removeAllItem()
    {
        items.clear();
    }

signals:

    void itemsChanged();

private:
    QVariantList items;
    Q_PROPERTY(QVariantList items READ getItems WRITE setItems NOTIFY itemsChanged FINAL)
};

#endif // LISTMODEL_H
