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
    //QML中点击后,通知C++处理,添加Q_INVOKABLE QML文件即可访问
    Q_INVOKABLE void itemClicked();
    int getWindowWidth() const;
    void setWindowWidth(int newWindowWidth);

    int getWindowHeight() const;
    void setWindowHeight(int newWindowHeight);

signals:
    void cellWidthChanged();

    void cellHeightChanged();

    void screenImageChanged();

    void windowWidthChanged();

    void windowHeightChanged();

public slots:
    void receiveSignalFromQMLFile(int i, QString str, QString str2);
private:
    //定义的属性QML中可以访问，C++可以拿到数据
    int cellWidth;
    int cellHeight;
    int windowWidth;
    int windowHeight;
    QString screenImage;
    //NOTIFY值修改，通知
    Q_PROPERTY(int cellWidth READ getCellWidth WRITE setCellWidth NOTIFY cellWidthChanged FINAL)
    Q_PROPERTY(int cellHeight READ getCellHeight WRITE setCellHeight NOTIFY cellHeightChanged FINAL)
    Q_PROPERTY(QString screenImage READ getScreenImage WRITE setScreenImage NOTIFY screenImageChanged FINAL)
    Q_PROPERTY(int windowWidth READ getWindowWidth WRITE setWindowWidth NOTIFY windowWidthChanged FINAL)
    Q_PROPERTY(int windowHeight READ getWindowHeight WRITE setWindowHeight NOTIFY windowHeightChanged FINAL)
};

#endif // QMLSIZEMANAGER_H
