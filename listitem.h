#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QtQml>
#include "global.h"

class ListItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT //声明QML可以访问元素
public:
    explicit ListItem(QObject* parent = nullptr) {};
    explicit ListItem(S_PHONE_INFO* info, QObject *parent = nullptr);
    //单实例
    static ListItem* getInstance();

    int getIndex() const;
    void setIndex(int newIndex);

    int getPhoneId() const;
    void setPhoneId(int newPhoneId);

    QString getPhoneName() const;
    void setPhoneName(const QString &newPhoneName);

    QString getPhoneInstanceNo() const;
    void setPhoneInstanceNo(const QString &newPhoneInstanceNo);

    bool getCheckBox() const;
    void setCheckBox(bool newCheckBox);

    QString getImagePath() const;
    void setImagePath(const QString &newImagePath);

    S_PHONE_INFO getPhoneInfo() const;
    void setPhoneInfo(const S_PHONE_INFO &newPhoneInfo);

    int getAuthorStatus() const;
    void setAuthorStatus(int newAuthorStatus);

    bool getBShowAuthorImg() const;
    void setBShowAuthorImg(bool newBShowAuthorImg);

    QString getExpireTime() const;
    void setExpireTime(const QString &newExpireTime);

signals:

    void indexChanged();

    void phoneIdChanged();

    void phoneNameChanged();

    void phoneInstanceNoChanged();

    void checkBoxChanged();

    void ImagePathChanged();

    void PhoneInfoChanged();

    void authorStatusChanged();

    void bShowAuthorImgChanged();

    //用于处理点击事件，包含左键点击和右键菜单
    //void ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu);
    void ExpireTimeChanged();

public slots:
    //用于处理点击事件，包含左键点击和右键菜单
    void ShowInstanceSignalFromQMLFile(int iId,QString strPhoneName,QString strInstanceNo,QString strExpireTime,bool bIsShowMenu);

private:
    
    //显示实例,将显示实例部分从MainWindow中提取出来
    void on_ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu);
private:
    int index;          //序列号
    int phoneId;        //手机编号
    QString phoneName; //手机名
    QString phoneInstanceNo; //实例No
    QString ImagePath;          //图片路径
    QString ExpireTime;         //到期事件
    //QString AuthorImgPath;      //授权状态图片
    bool bShowAuthorImg;        //是否显示授权
    bool checkBox; //checkBox选中状态
    int  authorStatus; //授权状态0：没有授权，1：已授权，2：被授权
    S_PHONE_INFO PhoneInfo;
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged FINAL)
    Q_PROPERTY(int phoneId READ getPhoneId WRITE setPhoneId NOTIFY phoneIdChanged FINAL)
    Q_PROPERTY(QString phoneName READ getPhoneName WRITE setPhoneName NOTIFY phoneNameChanged FINAL)
    Q_PROPERTY(QString phoneInstanceNo READ getPhoneInstanceNo WRITE setPhoneInstanceNo NOTIFY phoneInstanceNoChanged FINAL)
    Q_PROPERTY(bool checkBox READ getCheckBox WRITE setCheckBox NOTIFY checkBoxChanged FINAL)
    Q_PROPERTY(QString ImagePath READ getImagePath WRITE setImagePath NOTIFY ImagePathChanged FINAL)
    Q_PROPERTY(S_PHONE_INFO PhoneInfo READ getPhoneInfo WRITE setPhoneInfo NOTIFY PhoneInfoChanged FINAL)
    Q_PROPERTY(int authorStatus READ getAuthorStatus WRITE setAuthorStatus NOTIFY authorStatusChanged FINAL)
    Q_PROPERTY(bool bShowAuthorImg READ getBShowAuthorImg WRITE setBShowAuthorImg NOTIFY bShowAuthorImgChanged FINAL)
    Q_PROPERTY(QString ExpireTime READ getExpireTime WRITE setExpireTime NOTIFY ExpireTimeChanged FINAL)
};

#endif // LISTITEM_H
