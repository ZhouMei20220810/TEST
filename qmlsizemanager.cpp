#include "qmlsizemanager.h"
#include <QMessageBox>

QMLSizeManager::QMLSizeManager(QObject *parent)
    : QObject{parent}
{}

QMLSizeManager *QMLSizeManager::getInstance()
{
    //单实例对象
    static QMLSizeManager* object = new QMLSizeManager();
    return object;
}

int QMLSizeManager::getCellWidth() const
{
    return cellWidth;
}

void QMLSizeManager::setCellWidth(int newCellWidth)
{
    if (cellWidth == newCellWidth)
        return;
    cellWidth = newCellWidth;
    emit cellWidthChanged();
}

int QMLSizeManager::getCellHeight() const
{
    return cellHeight;
}

void QMLSizeManager::setCellHeight(int newCellHeight)
{
    if (cellHeight == newCellHeight)
        return;
    cellHeight = newCellHeight;
    emit cellHeightChanged();
}

QString QMLSizeManager::getScreenImage() const
{
    return screenImage;
}

void QMLSizeManager::setScreenImage(const QString &newScreenImage)
{
    if (screenImage == newScreenImage)
        return;
    screenImage = newScreenImage;
    emit screenImageChanged();
}

void QMLSizeManager::itemClicked()
{
    qDebug() << "QML 调用 C++中的函数";
    //QMessageBox::information(NULL, "tishi", "QML 调用 C++中的函数", QMessageBox::Ok);

}

void QMLSizeManager::receiveSignalFromQMLFile(int i, QString str, QString str2)
{
    qDebug() << "receiveSignalFromQMLFile 收到来自QML的信号"<<i<<str<<str2;
}

int QMLSizeManager::getWindowHeight() const
{
    return windowHeight;
}

void QMLSizeManager::setWindowHeight(int newWindowHeight)
{
    if (windowHeight == newWindowHeight)
        return;
    windowHeight = newWindowHeight;
    emit windowHeightChanged();
}

int QMLSizeManager::getWindowWidth() const
{
    return windowWidth;
}

void QMLSizeManager::setWindowWidth(int newWindowWidth)
{
    if (windowWidth == newWindowWidth)
        return;
    windowWidth = newWindowWidth;
    emit windowWidthChanged();
}
