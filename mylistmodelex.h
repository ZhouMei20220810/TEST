#ifndef MYLISTMODELEX_H
#define MYLISTMODELEX_H

#include <QAbstractListModel>
//是不是直接可以改成S_PHONE_INFO
class MyData
{
public:
    MyData(bool bChecked,bool bShowAuthImg, int iPhoneId,int iAuthorStatus,QString strPhoneName,QString strInstanceNo,QString strImagePath,QString strExpireTime)
        :m_bChecked(bChecked),m_bShowAuthImg(bShowAuthImg),m_iPhoneId(iPhoneId)
        ,m_iAuthorStatus(iAuthorStatus),m_strPhoneName(strPhoneName),m_strInstanceNo(strInstanceNo)
        ,m_strImagePath(strImagePath),m_strExpireTime(strExpireTime)
    {

    }
    bool    m_bChecked;           //是否勾选
    bool    m_bShowAuthImg;       //是否显示授权状态,没有授权：false 已授权/被授权:true
    int     m_iPhoneId;           //手机Id
    int     m_iAuthorStatus;      //授权状态1：已授权，2：被授权
    QString m_strPhoneName;       //手机名称
    QString m_strInstanceNo;      //手机示例
    QString m_strImagePath;       //手机截图
    QString m_strExpireTime;      //到期时间
};

class MyListModelEx : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MyRoleName
    {
        Name = Qt::DisplayRole+1,
        InstanceNo,
        ImagePath
    };
    explicit MyListModelEx(QObject *parent = nullptr);

    static MyListModelEx* getInstance();
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

public slots:
    void ShowInstanceSignalFromQMLFile(int iId, QString strPhoneName, QString strInstanceNo, QString strExpireTime, bool bIsShowMenu);
    void do_ItemClickSignals(int index, const QVariant& data);
    Q_INVOKABLE void do_notifyRefreshWindow();
private:
    //QList<QString> m_data;
    QList<MyData> m_data;
    //QHash<int,QByteArray> m_roles;
};

#endif // MYLISTMODELEX_H
