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
public slots:
    void do_UpdateRenewActiveCodeSignals(int iPhoneId, QString strRenewActiveCode);
    void do_UpdateActiveCodeStatusSignals(QString strRenewAcitiveCode, bool bSuccess);
private:
    Ui::ActiveCodeRenewItem *ui;
    S_PHONE_INFO m_phoneInfo;
    bool         m_bSuccessed;
};

#endif // ACTIVECODERENEWITEM_H
