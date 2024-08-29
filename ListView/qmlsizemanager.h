#ifndef QMLSIZEMANAGER_H
#define QMLSIZEMANAGER_H

#include <QObject>
#include <QtQml>

class QMLSizeManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT //声明QML可以访问元素
public:
    explicit QMLSizeManager(QObject *parent = nullptr);
    //单实例
    static QMLSizeManager* getInstance();
    int getCellWidth() const;
    void setCellWidth(int newCellWidth);

    int getCellHeight() const;
    void setCellHeight(int newCellHeight);

    QString getScreenImage() const;
    void setScreenImage(const QString &newScreenImage);

signals:
    void cellWidthChanged();

    void cellHeightChanged();

    void screenImageChanged();

private:
    //定义的属性QML中可以访问，C++可以拿到数据
    int cellWidth;
    int cellHeight;
    QString screenImage;
    //NOTIFY值修改，通知
    Q_PROPERTY(int cellWidth READ getCellWidth WRITE setCellWidth NOTIFY cellWidthChanged FINAL)
    Q_PROPERTY(int cellHeight READ getCellHeight WRITE setCellHeight NOTIFY cellHeightChanged FINAL)
    Q_PROPERTY(QString screenImage READ getScreenImage WRITE setScreenImage NOTIFY screenImageChanged FINAL)
};

#endif // QMLSIZEMANAGER_H
