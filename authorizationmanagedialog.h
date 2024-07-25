#ifndef AUTHORIZATIONMANAGEDIALOG_H
#define AUTHORIZATIONMANAGEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class AuthorizationManageDialog;
}

class AuthorizationManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizationManageDialog(QWidget *parent = nullptr);
    ~AuthorizationManageDialog();
signals:
    void notifyMainWindowRefreshGroupListSignals();
private slots:
    void on_btnClose_clicked();

    void on_toolBtnAddAuthorization_clicked();

    void on_toolBtnAuthorized_clicked();

    void on_toolBtnBeAuthorized_clicked();

    void on_btnAuthorPolicy_clicked();

    void on_btnUserPolicy_clicked();

    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_toolBtnRefresh_clicked();

private:
    void InitListWidget();
    void HttpPostAddAuthCode(QString strAuthCode);

    void HttpGetAuthorizedListInfo(bool bIsAuth,int iPage, int iPageSize);
    void LoadAuthorizedList(QMap<int, S_AUTHOR_INFO> map);
    void LoadBeAuthorizedList(QMap<int, S_AUTHOR_INFO> map);
    void LoadResultInfo(QMap<int, S_AUTH_RESULT_INFO> map);

    QPixmap generateCaptchaImage(const QString& code);
    void RefreshPictureCode();

    Ui::AuthorizationManageDialog *ui;

    QString m_strPictureCode;
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // AUTHORIZATIONMANAGEDIALOG_H
