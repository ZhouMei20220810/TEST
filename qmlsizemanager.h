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
    
    int getWindowWidth() const;
    void setWindowWidth(int newWindowWidth);

    int getWindowHeight() const;
    void setWindowHeight(int newWindowHeight);

    bool getItemVerticalScreen() const;
    void setItemVerticalScreen(bool newItemVerticalScreen);

signals:
    void cellWidthChanged();

    void cellHeightChanged();

    void screenImageChanged();

    void windowWidthChanged();

    void windowHeightChanged();

    void itemVerticalScreenChanged();

public slots:
    void receiveSignalFromQMLFile(int i, QString str, QString str2);
private:
    //定义的属性QML中可以访问，C++可以拿到数据
    int cellWidth;
    int cellHeight;
    int windowWidth;
    int windowHeight;
    bool itemVerticalScreen;        //true竖屏,false横屏
    //NOTIFY值修改，通知
    Q_PROPERTY(int cellWidth READ getCellWidth WRITE setCellWidth NOTIFY cellWidthChanged FINAL)
    Q_PROPERTY(int cellHeight READ getCellHeight WRITE setCellHeight NOTIFY cellHeightChanged FINAL)
    Q_PROPERTY(int windowWidth READ getWindowWidth WRITE setWindowWidth NOTIFY windowWidthChanged FINAL)
    Q_PROPERTY(int windowHeight READ getWindowHeight WRITE setWindowHeight NOTIFY windowHeightChanged FINAL)
    Q_PROPERTY(bool itemVerticalScreen READ getItemVerticalScreen WRITE setItemVerticalScreen NOTIFY itemVerticalScreenChanged FINAL)
};

#endif // QMLSIZEMANAGER_H
