#ifndef TRANSFERPHONEDIALOG_H
#define TRANSFERPHONEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class TransferPhoneDialog;
}

class TransferPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferPhoneDialog(QMap<int, S_PHONE_INFO> mapPhoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent = nullptr);
    ~TransferPhoneDialog();

private slots:
    void on_btnClose_clicked();
    void on_btnOk_clicked();

    void on_btnAuthorPolicy_clicked();

    void on_checkBoxAll_clicked(bool checked);

private:
    void LoadWidgetData(QMap<int, S_PHONE_INFO> mapPhoneInfo);

    QString generateRandomCode(int length = 4);
    void RefreshPictureCode();
    QPixmap generateCaptchaImage(const QString& code);

    void HttpPostTransferPhone(QMap<int, int> mapId);
private:
    Ui::TransferPhoneDialog *ui;
    QMap<int, S_LEVEL_INFO> m_mapLevelList;
    QString m_strPictureCode;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // TRANSFERPHONEDIALOG_H
