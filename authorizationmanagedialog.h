#ifndef AUTHORIZATIONMANAGEDIALOG_H
#define AUTHORIZATIONMANAGEDIALOG_H

#include <QDialog>

namespace Ui {
class AuthorizationManageDialog;
}

class AuthorizationManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorizationManageDialog(QWidget *parent = nullptr);
    ~AuthorizationManageDialog();

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
    QString generateRandomCode(int length = 4);
    QPixmap generateCaptchaImage(const QString& code);

    Ui::AuthorizationManageDialog *ui;

    QString m_strPictureCode;
};

#endif // AUTHORIZATIONMANAGEDIALOG_H
