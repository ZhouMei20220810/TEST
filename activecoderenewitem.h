#ifndef ACTIVECODERENEWITEM_H
#define ACTIVECODERENEWITEM_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ActiveCodeRenewItem;
}

class ActiveCodeRenewItem : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveCodeRenewItem(S_PHONE_INFO phoneInfo,QWidget *parent = nullptr);
    ~ActiveCodeRenewItem();

    void setCheckBoxStatus(bool bCheck);
    bool getCheckBoxStatus();

    void setRenewActiveCode(QString strRenewActiveCode);
    QString getRenewActiveCode();

    int getStatus();
public slots:
    void do_UpdateRenewActiveCodeSignals(int iPhoneId, QString strRenewActiveCode);
    void do_activeCodeStatusSignals(QString strRenewAcitiveCode, bool bSuccess, QString strStatus);
private:
    Ui::ActiveCodeRenewItem *ui;
    S_PHONE_INFO m_phoneInfo;
    int             m_iStatus;
};

#endif // ACTIVECODERENEWITEM_H
