#include "transferphonedialog.h"
#include "ui_transferphonedialog.h"
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QRandomGenerator>
#include "messagetips.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "policydialog.h"
#include "transfertipsdialog.h"
#define  PICTURE_CODE_WIDTH     110
#define  PICTURE_CODE_HEIGHT    32

TransferPhoneDialog::TransferPhoneDialog(QMap<int, S_PHONE_INFO> mapPhoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransferPhoneDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("转移云手机");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);
    m_iCurSelCount = 0;
    m_mapLevelList = mapLevelList;

    ui->listWidget->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(0);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    LoadWidgetData(mapPhoneInfo);
    RefreshPictureCode();
    ui->checkBoxAll->setText(QString("%1/%2         名称").arg(0).arg(mapPhoneInfo.size()));
    ui->labelPictureCode->installEventFilter(this);
}

// 生成随机字符串
QString TransferPhoneDialog::generateRandomCode(int length/* = 4*/)
{
    const QString possibleChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString code;
    QRandomGenerator* generator = QRandomGenerator::global();
    for (int i = 0; i < length; ++i) {
        code.append(possibleChars.at(generator->generate() % possibleChars.size()));
    }
    return code;
}

void TransferPhoneDialog::RefreshPictureCode()
{
    // 生成验证码字符串
    m_strPictureCode = generateRandomCode(4);
    qDebug() << "Piccode = " << m_strPictureCode;
    // 生成验证码图像
    QPixmap captchaImg = generateCaptchaImage(m_strPictureCode);
    ui->labelPictureCode->setPixmap(captchaImg.scaled(QSize(PICTURE_CODE_WIDTH, PICTURE_CODE_HEIGHT), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}
TransferPhoneDialog::~TransferPhoneDialog()
{
    delete ui;
}

void TransferPhoneDialog::on_btnClose_clicked()
{
    this->close();
}

void TransferPhoneDialog::LoadWidgetData(QMap<int, S_PHONE_INFO> mapPhoneInfo)
{
    ui->listWidget->clear();
    if (mapPhoneInfo.size() <= 0)
        return;

    QString strStyleSheet = "QCheckBox{padding-left:7px;spacing:5px;}QCheckBox::indicator{width:16px;height:16px;}QCheckBox::indicator:unchecked{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:hover{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:pressed{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:checked{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:hover{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:pressed{image:url(:/login/resource/login/option_select.png);}";
    QListWidgetItem* item = NULL;
    QCheckBox* checkBox = NULL;
    QString strLevelName;
    QMap<int, S_LEVEL_INFO>::iterator iterFind;
    QMap<int, S_PHONE_INFO>::iterator iter = mapPhoneInfo.begin();
    for (; iter != mapPhoneInfo.end(); iter++)
    {
        iterFind = m_mapLevelList.find(iter->iLevel);
        strLevelName = "";
        if (iterFind != m_mapLevelList.end())
        {
            strLevelName = iterFind->strLevelName;
        }

        checkBox = new QCheckBox(this);
        checkBox->setStyleSheet(strStyleSheet);
        checkBox->setIcon(QIcon(QString(":/main/resource/main/%1.png").arg(strLevelName)));
        if (iter->strName.isEmpty())
            checkBox->setText(iter->strInstanceNo);
        else
            checkBox->setText(iter->strName);
        checkBox->setFixedSize(340, LISTMODE_ITEM_HEGITH);
        connect(checkBox, &QCheckBox::clicked, this, &TransferPhoneDialog::do_historyItemCheckBoxStatus);
        //widget2 = new PhoneListModeItemWidget(phoneInfo, this);
        //connect(widget2, &PhoneListModeItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
        //connect(widget2, &PhoneListModeItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
        item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
        item->setData(Qt::UserRole, QVariant::fromValue(*iter));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, checkBox);
    }
}

void TransferPhoneDialog::do_historyItemCheckBoxStatus(bool checked)
{
    qDebug() << "TransferPhoneDialog do_historyItemCheckBoxStatus 改变状态=" << checked;
    if (checked)
    {
        m_iCurSelCount++;
    }
    else
    {
        m_iCurSelCount--;
    }
    int iCount = ui->listWidget->count();
    ui->checkBoxAll->setText(QString("%1/%2         名称").arg(m_iCurSelCount).arg(iCount));
    ui->checkBoxAll->setChecked(m_iCurSelCount == iCount ? true : false);
}

// 生成验证码图像
QPixmap TransferPhoneDialog::generateCaptchaImage(const QString& code)
{
    QPixmap captcha(PICTURE_CODE_WIDTH, PICTURE_CODE_HEIGHT);
    captcha.fill(QColor(215, 215, 222));

    QPainter painter(&captcha);
    QFont font("Arial", 12);
    painter.setFont(font);

    QColor textColor;
    int iVerOffset = 0;
    for (int i = 0; i < code.length(); ++i) {
        textColor.setHsv((i * 150) % 360, 255, 150); // 随机颜色
        painter.setPen(textColor);
        //添加垂直随机偏移量
        iVerOffset = QRandomGenerator::global()->bounded(-5, 5);
        //绘制字符，同时应用水平和垂直偏移
        painter.drawText(15 + i * 20, 20 + iVerOffset, code.at(i)); // 分散字符以避免被轻易识别
    }

    // 可以增加噪声线或点以提高安全性
    /*for (int i = 0; i < 50; ++i) {
        painter.drawLine(QPoint(QRandomGenerator::global()->bounded(0, PICTURE_CODE_WIDTH), QRandomGenerator::global()->bounded(0, PICTURE_CODE_HEIGHT)),
            QPoint(QRandomGenerator::global()->bounded(0, PICTURE_CODE_WIDTH), QRandomGenerator::global()->bounded(0, PICTURE_CODE_HEIGHT)));
    }*/

    painter.end();
    return captcha;
}
void TransferPhoneDialog::on_btnOk_clicked()
{
    //确定转移
    qDebug() << "确定转移";    
    int iCount = ui->listWidget->count();
    if (iCount <= 0)
    {
        MessageTips* tips = new MessageTips("数据列表为空", this);
        tips->show();
        return;
    }
    //行-手机id
    QMap<int, int> map;
    QCheckBox* checkBox = NULL;
    QListWidgetItem* item = NULL;
    S_PHONE_INFO phoneInfo;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidget->item(i);
        if (item != NULL)
        {
            checkBox = qobject_cast<QCheckBox*>(ui->listWidget->itemWidget(item));
            if (checkBox != NULL && checkBox->isChecked())
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                map.insert(i, phoneInfo.iId);
            }
        }
    }
    if (map.size() <= 0)
    {
        MessageTips* tips = new MessageTips("暂无勾选数据", this);
        tips->show();
        return;
    }

    QString strPhoneOrAccount = ui->lineEditPhoneOrAccount->text();
    if (strPhoneOrAccount.isEmpty())
    {
        MessageTips* tips = new MessageTips("接收方账号不能为空", this);
        tips->show();
        return;
    }
    QString strInputCode = ui->lineEditPictureCode->text();
    if (strInputCode.compare(m_strPictureCode, Qt::CaseInsensitive) != 0)
    {
        MessageTips* dialog = new MessageTips("验证码不一致", this);
        dialog->show();
        return;
    }

    bool bCheck = ui->checkBoxPolicy->isChecked();
    if (!bCheck)
    {
        MessageTips* dialog = new MessageTips("请先阅读并勾选", this);
        dialog->show();
        return;
    }

    TransferTipsDialog* tips = new TransferTipsDialog(map.size(),strPhoneOrAccount);
    if (QDialog::Accepted == tips->exec())
    {
        HttpPostTransferPhone(strPhoneOrAccount,map);
    }
}

void TransferPhoneDialog::HttpPostTransferPhone(QString strPhoneOrAccount,QMap<int, int> mapId)
{
    int iSize = mapId.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_TRANSFER_PHONE;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit());
    request.setUrl(url);
    QJsonObject jsonObj;
    jsonObj.insert("mobile", strPhoneOrAccount);

    QJsonArray listArray;
    QMap<int, int>::iterator iter = mapId.begin();
    for (; iter != mapId.end(); iter++)
    {
        listArray.append(iter.value());
    }
    jsonObj["ids"] = listArray;
    QJsonDocument doc(jsonObj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();

                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    QJsonObject data = obj["data"].toObject();
                    int iSuccesssCount = data["snum"].toInt();                    
                    qDebug() << "成功台数=" << iSuccesssCount;
                    //转移成功之后关闭窗口
                    this->close();
                    emit TransferSuccessRefreshInstanceListSignals();
                    //不用更新状态，需要统计成功多少台
                    //LoadReplaceInstanceStatus(map);
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
void TransferPhoneDialog::on_btnAuthorPolicy_clicked()
{
    //易舜转移功能服务协议
    PolicyDialog* policy = new PolicyDialog("易舜转移功能服务协议", "https://www.ysyos.com/deal/Transfer.html", this);
    policy->exec();
}

bool TransferPhoneDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->labelPictureCode)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* e = static_cast<QMouseEvent*>(event);
            switch (e->button())
            {
            case Qt::LeftButton:
                RefreshPictureCode();
                return true;
                break;
            default:
                break;
            }
        }
        else
        {
            return false;
        }
    }
    return QDialog::eventFilter(watched, event);
}

void TransferPhoneDialog::on_checkBoxAll_clicked(bool checked)
{
    int iCount = ui->listWidget->count();
    if (iCount <= 0)
    {
        return;
    }

    QListWidgetItem* item = NULL;
    QCheckBox* checkBox = NULL;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidget->item(i);
        if (item != NULL)
        {
            checkBox = static_cast<QCheckBox*>(ui->listWidget->itemWidget(item));
            if (checkBox != NULL)
            {
                checkBox->setChecked(checked);
            }
        }
    }
    if (checked)
    {
        m_iCurSelCount = iCount;
        ui->checkBoxAll->setText(QString("%1/%2         名称").arg(iCount).arg(iCount));
    }        
    else
    {
        m_iCurSelCount = 0;
        ui->checkBoxAll->setText(QString("%1/%2         名称").arg(0).arg(iCount));
    }        
}


void TransferPhoneDialog::on_btnCancel_clicked()
{
    this->close();
}

