#include "individualcenterwidget.h"
#include "ui_individualcenterwidget.h"
#include "forgetpwdialog.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include "messagetips.h"
#include "buyhistorywidget.h"
#include "authorizationmanagedialog.h"
#include "activecodehistorydialog.h"
#include <QGraphicsDropShadowEffect>
#include "transferphonehistorydialog.h"

IndividualCenterWidget::IndividualCenterWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IndividualCenterWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    QFontMetrics fontWidth(ui->labelAccount->font());
    QString strElideNote = fontWidth.elidedText(GlobalData::strAccount, Qt::ElideRight, 90);
    ui->labelAccount->setText(strElideNote);
    //ui->labelAccount->setText(GlobalData::strAccount);
    ui->labelAccount->setToolTip(GlobalData::strAccount);
    ui->labelID->setText(QString("%1").arg(GlobalData::id));

}

IndividualCenterWidget::~IndividualCenterWidget()
{
    delete ui;
}

void IndividualCenterWidget::on_toolBtnIndividualCenter_clicked()
{
    //个人中心,修改密码
    ForgetPWDialog* forgetPW = new ForgetPWDialog();
    forgetPW->setModal(true);
    forgetPW->show();

    this->hide();
}

void IndividualCenterWidget::on_toolBtnLogout_clicked()
{
    //发送信号给主窗口注销登录，否则会重复注销
    emit logoutSignals();
}


void IndividualCenterWidget::on_toolBtnBuyHistory_clicked()
{
    BuyHistoryWidget* widget = new BuyHistoryWidget();
    widget->setModal(true);
    widget->show();

    this->hide();
}

void IndividualCenterWidget::on_toolBtnAuthorizationManage_clicked()
{
    AuthorizationManageDialog* dialog = new AuthorizationManageDialog();
    connect(dialog, &AuthorizationManageDialog::notifyMainWindowRefreshGroupListSignals, this, &IndividualCenterWidget::notifyMainWindowRefreshGroupListSignals);
    dialog->setModal(true);
    dialog->show();

    this->hide();
}

void IndividualCenterWidget::on_toolBtnActiveCodeHistory_clicked()
{
    //激活码记录
    ActiveCodeHistoryDialog* dialog = new ActiveCodeHistoryDialog();
    dialog->exec();
}


void IndividualCenterWidget::on_toolBtnTransferHistory_clicked()
{
    //转移记录
    TransferPhoneHistoryDialog* dialog = new TransferPhoneHistoryDialog();
    dialog->exec();
}

