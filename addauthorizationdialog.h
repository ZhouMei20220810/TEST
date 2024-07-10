#ifndef ADDAUTHORIZATIONDIALOG_H
#define ADDAUTHORIZATIONDIALOG_H

#include <QDialog>
#include "global.h"
#include <QButtonGroup>

namespace Ui {
class AddAuthorizationDialog;
}

class AddAuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAuthorizationDialog(S_PHONE_INFO phoneInfo,QWidget *parent = nullptr);
    ~AddAuthorizationDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();

    void on_toolBtnTips_clicked();

    void on_radioButtonAccount_clicked(bool checked);

    void on_radioButtonAuthorCode_clicked(bool checked);

    void on_radioButtonReadOnly_clicked(bool checked);

    void on_radioButtonControl_clicked(bool checked);

    void on_btnAuthorPolicy_clicked();

    void on_btnCancel_3_clicked();

    void on_btnCancelAuthAccount_3_clicked();

    void on_btnCancel_2_clicked();

    void on_btnCancelAuthAccount_2_clicked();
    void on_toolBtnAuthCode_clicked();

private:
    void HttpPostGeneratorAuthCode(bool bIsReadOnly,qint64 iUserInstanceId, qint64 iExpireTime);
    void HttpPostAuthAccountByPhone(bool bIsReadOnly, int iUserInstanceId, qint64 iExpireTime,QString strPhoneNum);
private:
    Ui::AddAuthorizationDialog *ui;

    bool m_bIsAccountAuth;
    bool m_bIsReadOnly;
    S_PHONE_INFO m_phoneInfo;
    QButtonGroup* m_btnGroup;
    int m_iDay;
};

#endif // ADDAUTHORIZATIONDIALOG_H
