#ifndef MYLISTMODELEX_H
#define MYLISTMODELEX_H
#include <QAbstractListModel>
#include "listitem.h"
#include "mainwindow.h"
//是不是直接可以改成S_PHONE_INFO
class MyListModelEx : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        /*CheckedRole = Qt::UserRole + 1 */
        NameRole = Qt::UserRole + 1,
        InstanceNoRole,
        ImagePathRole,
        CheckedRole,
        IsShowAuthImgRole,
        //PhoneIdRole,
        AuthorStatusRole,
        //ExpireTimeRole,
        ItemIndexRole,
        PhoneInfoRole
    };

    explicit MyListModelEx(QObject *parent = nullptr);

    static MyListModelEx* getInstance(MainWindow *pMainWindows);
    // Header:
    //ListModel基本用不到
    /*QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;*/

    // Basic functionality:
    //最简单的model需要实现下面三个函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    
    Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void addItem(MainWindow* mainWindow,S_PHONE_INFO info);
    //移除所有项
    void removeAllItem();
    //取消选择
    int setCancelSelectCheckBox(bool bChecked);
    //全选
    int setAllCheckBox(bool newCheckBox);
    //反选,返回选中条数
    int setFanXuanCheckBox();
    //设置新的图片路径
    void setNewImagePath(int itemIndex, QString strNewImagePath);
    //设置实例名称
    void setNewPhoneName(int iPhoneId, QString strNewPhoneName);

    QList<ListItem*> itemList()
    {
        return items;
    }
    //通过下标获取ListItem
    ListItem* getListItemByIndex(int index);

    //获取选中条数
    int getListItemCheckedCount();

    //鼠标点击事件
    Q_INVOKABLE void mousePressEvent(const QVariantMap& event);
    //滚动条事件
    Q_INVOKABLE void do_onContentXYChanged();
public slots:
    //暂时没有用到，记录一下QML中发送信号，C++槽函数响应
    void ShowInstanceSignalFromQMLFile(QString strPhoneName, QString strInstanceNo, bool bIsShowMenu, S_PHONE_INFO info);
    //void do_itemClicked(int index);
    //QML中点击后,通知C++处理,添加Q_INVOKABLE QML文件即可访问
    //连接到QML的信号
    Q_INVOKABLE void onCheckBoxChanged(int index,bool bChecked);
signals:
    //qml点击事件通过QmlSendMainWindowSignals信号通知到MainWindow
    void QmlSendMainWindowSignals(S_PHONE_INFO info, bool bShowMenu);

    void QmlCheckBoxSignals(bool bChecked);
public:
    int iItemIndex;
    QList<ListItem*> items;   

    MainWindow* m_MainWindow;
};

// 下载图片的线程任务
class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QNetworkAccessManager* manager,MainWindow* mainWindow, QObject* parent = nullptr)
        : QObject(parent), m_pMainWindow(mainWindow), m_manager(manager)
    {
        m_model = MyListModelEx::getInstance(mainWindow);
    }

    void downloadImages(QMap<QString, S_TASK_INFO> mapTask)
    {
        QMap<QString, S_TASK_INFO>::iterator iterFind;
        for (auto& item : m_model->items) 
        {
            iterFind = mapTask.find(item->getPhoneInstanceNo());
            if (iterFind == mapTask.end())
            {
                continue;                
            }
            //if (!item.image.isNull())
            //    continue;
            if (iterFind->strUrl.isEmpty())
            {
                qDebug() << "图片下载链接为空";
                continue;
            }

            QString strUrl = iterFind->strUrl;
            QNetworkRequest request(strUrl);
            QNetworkReply* reply = m_manager->get(request);           
            connect(reply, &QNetworkReply::finished, this, [this, reply, item, strUrl]() {
                if (reply->error() == QNetworkReply::NoError) {
                    /*QByteArray imageData = reply->readAll();
                    QPixmap pixmap;
                    pixmap.loadFromData(imageData);   */                 

                    //判断之前的文件是否存在
                    QString strPicturePath = GlobalData::strFileTempDir + item->getPhoneInstanceNo() + ".png";
                    if (QFile::exists(strPicturePath))
                    {
                        if (!QFile::remove(strPicturePath))
                        {
                            qDebug() << "remove fail:" << item->getImagePath();
                        }
                    }

                    QString strTempImagePath = item->getImagePath();
                    if (QFile::exists(strTempImagePath))
                    {
                        if (!QFile::rename(strTempImagePath, strPicturePath))
                        {
                            qDebug() << "rename fail: " << strPicturePath;
                        }
                    }

                    QString strFileName = strUrl.right(strUrl.size() - strUrl.lastIndexOf('/') - 1);
                    qDebug() << "url=" << strUrl << "strFileName=" << strFileName;
                    strTempImagePath = GlobalData::strFileTempDir + item->getPhoneInstanceNo() + "/" + strFileName;

                    //QMetaObject::invokeMethod(m_model, "setImage", Qt::QueuedConnection,
                    //    Q_ARG(QUrl, item.url),
                    //    Q_ARG(QPixmap, pixmap));
                    QFile   file(strTempImagePath);
                    if (file.open(QIODevice::WriteOnly))
                    {
                        file.write(reply->readAll());
                        file.close();
                    }
                    else
                    {
                        qDebug() << "downloadImages 下载完毕,但写入文件失败.";
                    }
                    m_model->setNewImagePath(item->getItemIndex(), strTempImagePath);
                    //QMetaObject::invokeMethod(m_model, "setNewImagePath", Qt::QueuedConnection,
                    //    Q_ARG(int, item->getItemIndex()),
                    //    Q_ARG(QString, strTempImagePath));
                }
                else {
                    qDebug() << "Download failed:" << reply->errorString();
                }

                reply->deleteLater();
                });
        }
    }

signals:
    void imageDownloaded(QUrl, QPixmap);

private slots:
    void setImage(QString strNewImagePath)
    {
        for (auto& item : m_model->items) {
            //if (item.url == url) 
            {
                //item.setimage = pixmap;

                QModelIndex index = m_model->index(m_model->items.indexOf(item), 0);
                m_model->setData(index, strNewImagePath, MyListModelEx::ImagePathRole);
                //m_model->dataChanged(index, index, { MyListModelEx::ImagePathRole });
                break;
            }
        }
    }

private:
    MyListModelEx* m_model;
    MainWindow* m_pMainWindow;
    QNetworkAccessManager* m_manager;
};

#endif // MYLISTMODELEX_H
