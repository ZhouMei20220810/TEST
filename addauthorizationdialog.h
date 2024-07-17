#ifndef ADDAUTHORIZATIONDIALOG_H
#define ADDAUTHORIZATIONDIALOG_H

#include <QDialog>
#include "global.h"
#include <QButtonGroup>
#include "toolobject.h"

namespace Ui {
class AddAuthorizationDialog;
}

class AddAuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAuthorizationDialog(S_PHONE_INFO phoneInfo, QWidget* parent = nullptr);
    explicit AddAuthorizationDialog(QMap<int, S_PHONE_INFO> map,QWidget *parent = nullptr);
    ~AddAuthorizationDialog();

    void InitWidget(S_AUTHOR_INFO authInfo);
signals:
    void notifyMainWindowRefreshGroupListSignals();
private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();

    void on_radioButtonAccount_clicked(bool checked);

    void on_radioButtonAuthorCode_clicked(bool checked);

    void on_radioButtonReadOnly_clicked(bool checked);

    void on_radioButtonControl_clicked(bool checked);

    void on_btnAuthorPolicy_clicked();

    void on_btnCancel_3_clicked();
    void on_btnCancel_2_clicked();
    void on_toolBtnAuthCode_clicked();
    void on_btnCancelAuthCode_clicked();
    void on_btnCancelAuthAccount_clicked();

private:
    void InitAuthCodePage(S_AUTHOR_INFO authInfo);
    void InitAccountPage(S_AUTHOR_INFO authInfo);
    void HttpPostGeneratorAuthCode(bool bIsReadOnly,qint64 iUserInstanceId, qint64 iExpireTime);
    void HttpPostAuthAccountByPhone(bool bIsReadOnly, int iUserInstanceId, qint64 iExpireTime,QString strPhoneNum);
    void HttpPostBatchGeneratorAuthCode(bool bIsReadOnly, QMap<int, S_PHONE_INFO> map, qint64 iExpireTime);
    void HttpPostBatchAuthAccountByPhone(bool bIsReadOnly, QMap<int, S_PHONE_INFO> map, qint64 iExpireTime, QString strPhoneNum);
private:
    Ui::AddAuthorizationDialog *ui;

    bool m_bIsAccountAuth;
    bool m_bIsReadOnly;
    S_PHONE_INFO m_phoneInfo;
    QMap<int, S_PHONE_INFO> m_map;
    QButtonGroup* m_btnGroup;
    int m_iDay;
    int m_iInstanceId;
    ToolObject* m_toolObject;
};

#endif // ADDAUTHORIZATIONDIALOG_H
