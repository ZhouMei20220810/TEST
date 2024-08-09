#ifndef ONECLICKNEWMACHINEDIALOG_H
#define ONECLICKNEWMACHINEDIALOG_H

#include "global.h"

namespace Ui {
class OneClickNewMachineDialog;
}

class OneClickNewMachineDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit OneClickNewMachineDialog(QStringList strInstanceList,QWidget *parent = nullptr);
    ~OneClickNewMachineDialog();
private:
    void HttpGetModelList(QString strBrand, int iPage, int iPageSize);
    //根据品牌过滤
    void LoadBrandList(QMap<QString, S_BRAND_INFO> map);
    //更加品牌型号过滤
    void LoadModelList(QMap<QString, S_BRAND_INFO> map);
    void HttpPostOneClickNewMachine(QString strIMEICode, /*QString strAndroidID, */ QString strWIFIMAC, QString strPhoneOrder, int iModelId, QStringList strInstanceList);
private slots:
    void on_btnClose_clicked();

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_comboBoxBrand_currentIndexChanged(int index);

    void on_checkBoxIMEICode_clicked(bool checked);
    //void on_checkBoxAndroidID_clicked(bool checked);
    void on_checkBoxWIFIMAC_clicked(bool checked);
    void on_checkBoxPhoneOrder_clicked(bool checked);

private:
    Ui::OneClickNewMachineDialog *ui;
    bool        m_bIMEICode;
    //bool        m_bAndroidID;
    bool        m_bWIFIMAC;
    bool        m_bPhoneOrder;
    QStringList m_strInstanceList;
    QMap<QString, S_BRAND_INFO> m_mapBrand;
    QMap<QString, S_BRAND_INFO> m_mapModel;
};

#endif // ONECLICKNEWMACHINEDIALOG_H
