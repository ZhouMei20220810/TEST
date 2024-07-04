#include "authorizationmanagedialog.h"
#include "ui_authorizationmanagedialog.h"
#include "policydialog.h"
#include "messagetips.h"
#include <QRandomGenerator>
#include <QPainter>
#define  PICTURE_CODE_WIDTH     90
#define  PICTURE_CODE_HEIGHT    28

// 生成随机字符串
QString AuthorizationManageDialog::generateRandomCode(int length/* = 4*/)
{
    const QString possibleChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString code;
    QRandomGenerator* generator = QRandomGenerator::global();
    for (int i = 0; i < length; ++i) {
        code.append(possibleChars.at(generator->generate() % possibleChars.size()));
    }
    return code;
}

// 生成验证码图像
QPixmap AuthorizationManageDialog::generateCaptchaImage(const QString& code)
{
    QPixmap captcha(PICTURE_CODE_WIDTH, PICTURE_CODE_HEIGHT);
    captcha.fill(QColor(215, 215, 222));

    QPainter painter(&captcha);
    QFont font("Arial", 15);
    painter.setFont(font);

    QColor textColor;
    for (int i = 0; i < code.length(); ++i) {
        textColor.setHsv((i * 150) % 360, 255, 150); // 随机颜色
        painter.setPen(textColor);        
        //painter.drawText(20 + i * 40, 30, code.at(i)); // 分散字符以避免被轻易识别
        painter.drawText(5 + i * 20, 28, code.at(i)); // 分散字符以避免被轻易识别
    }

    // 可以增加噪声线或点以提高安全性
    /*for (int i = 0; i < 50; ++i) {
        painter.drawLine(QPoint(QRandomGenerator::global()->bounded(0, PICTURE_CODE_WIDTH), QRandomGenerator::global()->bounded(0, PICTURE_CODE_HEIGHT)),
            QPoint(QRandomGenerator::global()->bounded(0, PICTURE_CODE_WIDTH), QRandomGenerator::global()->bounded(0, PICTURE_CODE_HEIGHT)));
    }*/

    painter.end();
    return captcha;
}

AuthorizationManageDialog::AuthorizationManageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthorizationManageDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    // 生成验证码字符串
    m_strPictureCode = generateRandomCode();
    qDebug() << "Piccode = " << m_strPictureCode;
    // 生成验证码图像
    QPixmap captchaImg = generateCaptchaImage(m_strPictureCode);
    ui->labelPictureCode->setPixmap(captchaImg);//(captchaImg.scaled(QSize(PICTURE_CODE_WIDTH, PICTURE_CODE_HEIGHT), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

AuthorizationManageDialog::~AuthorizationManageDialog()
{
    delete ui;
}

void AuthorizationManageDialog::on_btnClose_clicked()
{
    this->close();
}


void AuthorizationManageDialog::on_toolBtnAddAuthorization_clicked()
{
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageAddAuthorization);

}


void AuthorizationManageDialog::on_toolBtnAuthorized_clicked()
{
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageAuthorized);
}


void AuthorizationManageDialog::on_toolBtnBeAuthorized_clicked()
{
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageAuthorized);
}


void AuthorizationManageDialog::on_btnAuthorPolicy_clicked()
{
    //易舜授权协议
    PolicyDialog* policy = new PolicyDialog("用户使用协议","https://www.ysyos.com/deal/Authorization.html",this);
    //policy->show();
    policy->exec();
}

void AuthorizationManageDialog::on_btnUserPolicy_clicked()
{
    //用户使用协议
    PolicyDialog* policy = new PolicyDialog("用户使用协议","https://www.ysyos.com/deal/UserPrivacy.html",this);
    //policy->show();
    policy->exec();
}


void AuthorizationManageDialog::on_btnOk_clicked()
{
    //确定
    QString strAuthCode = ui->lineEditAuthorCode->text();
    if (strAuthCode.isEmpty())
    {
        MessageTips* dialog = new MessageTips("授权码不能为空", this);
        dialog->show();
        return;
    }

    QString strInputCode = ui->lineEditPictureCode->text();
    if (strInputCode != m_strPictureCode)
    {
        MessageTips* dialog = new MessageTips("验证码不一致", this);
        dialog->show();
        return;
    }

    bool bCheck = ui->checkBoxPolicy->isChecked();
    if(!bCheck)
    {
        MessageTips* dialog = new MessageTips("请先阅读并勾选",this);
        dialog->show();
        return;
    }

    //添加授权操作
}


void AuthorizationManageDialog::on_btnCancel_clicked()
{
    //取消
    this->close();
}


void AuthorizationManageDialog::on_toolBtnRefresh_clicked()
{
    //刷新

}

