#include "listitem.h"

ListItem::ListItem(QObject *parent)
    : QObject{parent}
{}

//µ¥ÊµÀý
ListItem* ListItem::getInstance()
{
    static ListItem* listitem = new ListItem();
    return listitem;
}

int ListItem::getIndex() const
{
    return index;
}

void ListItem::setIndex(int newIndex)
{
    if (index == newIndex)
        return;
    index = newIndex;
    emit indexChanged();
}

int ListItem::getPhoneId() const
{
    return phoneId;
}

void ListItem::setPhoneId(int newPhoneId)
{
    if (phoneId == newPhoneId)
        return;
    phoneId = newPhoneId;
    emit phoneIdChanged();
}

QString ListItem::getPhoneName() const
{
    return phoneName;
}

void ListItem::setPhoneName(const QString &newPhoneName)
{
    if (phoneName == newPhoneName)
        return;
    phoneName = newPhoneName;
    emit phoneNameChanged();
}

QString ListItem::getPhoneInstanceNo() const
{
    return phoneInstanceNo;
}

void ListItem::setPhoneInstanceNo(const QString &newPhoneInstanceNo)
{
    if (phoneInstanceNo == newPhoneInstanceNo)
        return;
    phoneInstanceNo = newPhoneInstanceNo;
    emit phoneInstanceNoChanged();
}

bool ListItem::getCheckBox() const
{
    return checkBox;
}

void ListItem::setCheckBox(bool newCheckBox)
{
    if (checkBox == newCheckBox)
        return;
    checkBox = newCheckBox;
    emit checkBoxChanged();
}

bool ListItem::getAuthor() const
{
    return author;
}

void ListItem::setAuthor(bool newAuthor)
{
    if (author == newAuthor)
        return;
    author = newAuthor;
    emit authorChanged();
}

bool ListItem::getBeauthor() const
{
    return beauthor;
}

void ListItem::setBeauthor(bool newBeauthor)
{
    if (beauthor == newBeauthor)
        return;
    beauthor = newBeauthor;
    emit beauthorChanged();
}

QString ListItem::getImagePath() const
{
    return ImagePath;
}

void ListItem::setImagePath(const QString &newImagePath)
{
    if (ImagePath == newImagePath)
        return;
    ImagePath = newImagePath;
    emit ImagePathChanged();
}
