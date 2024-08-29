#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include <QtQml>

class ListItem : public QObject
{
    Q_OBJECT
    QML_ELEMENT //声明QML可以访问元素
public:
    explicit ListItem(QObject *parent = nullptr);
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

    bool getAuthor() const;
    void setAuthor(bool newAuthor);

    bool getBeauthor() const;
    void setBeauthor(bool newBeauthor);

    QString getImagePath() const;
    void setImagePath(const QString &newImagePath);

signals:

    void indexChanged();

    void phoneIdChanged();

    void phoneNameChanged();

    void phoneInstanceNoChanged();

    void checkBoxChanged();

    void authorChanged();

    void beauthorChanged();

    void ImagePathChanged();

private:
    int index;          //序列号
    int phoneId;        //手机编号
    QString phoneName; //手机名
    QString phoneInstanceNo; //实例No
    QString ImagePath;          //图片路径
    bool checkBox; //checkBox选中状态
    bool author; //授权
    bool beauthor; //被授权
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged FINAL)
    Q_PROPERTY(int phoneId READ getPhoneId WRITE setPhoneId NOTIFY phoneIdChanged FINAL)
    Q_PROPERTY(QString phoneName READ getPhoneName WRITE setPhoneName NOTIFY phoneNameChanged FINAL)
    Q_PROPERTY(QString phoneInstanceNo READ getPhoneInstanceNo WRITE setPhoneInstanceNo NOTIFY phoneInstanceNoChanged FINAL)
    Q_PROPERTY(bool checkBox READ getCheckBox WRITE setCheckBox NOTIFY checkBoxChanged FINAL)
    Q_PROPERTY(bool author READ getAuthor WRITE setAuthor NOTIFY authorChanged FINAL)
    Q_PROPERTY(bool beauthor READ getBeauthor WRITE setBeauthor NOTIFY beauthorChanged FINAL)
    Q_PROPERTY(QString ImagePath READ getImagePath WRITE setImagePath NOTIFY ImagePathChanged FINAL)
};

#endif // LISTITEM_H
