#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QFile>

PhoneItemWidget::PhoneItemWidget(S_TASK_INFO sTaskInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    m_sTaskInfo = sTaskInfo;
    /*QImage qrImage = generateAlipayQRCode(strQrCode);
    if (!qrImage.isNull())
    {
        QDir dir;
        GlobalData::strQrcode = dir.tempPath() + "/alipay_qrcode.png";
        qrImage.save(GlobalData::strQrcode);
        int width = ui->labelQrCode->width();
        int height = ui->labelQrCode->height();
        ui->labelQrCode->setPixmap(QPixmap(GlobalData::strQrcode).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        //正常情况
        ui->stackedWidget->setCurrentWidget(ui->pageQrCode);
    }*/
    
    m_strPicturePath = QDir::tempPath() + "/" + sTaskInfo.strPadCode + ".png";
    QFile file(m_strPicturePath);
    QString strUrl;
    if (!file.exists())
        strUrl = ":/main/resource/main/defaultSceenShot.png";

    int width = ui->label->width();
    int height = ui->label->height();

     //"https://zcpreview.armvm.com/screenshot/d9f9737fdcf0435faf2b32710ccb5ae4.png";
    ui->label->setPixmap(QPixmap(strUrl).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    // 使用QNetworkAccessManager获取网络图片
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(sTaskInfo.strUrl));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &PhoneItemWidget::do_DownloadImageFinish);
    qDebug() << "download image No="<<sTaskInfo.strPadCode<<"Url=" << sTaskInfo.strUrl;
    /*m_bDownload = false;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &ItemWidget::do_timeout);
    m_pTimer->start(2000);//2秒间隔
    */
}

PhoneItemWidget::PhoneItemWidget(S_PHONE_INFO sPhoneInfo, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_sPhoneInfo = sPhoneInfo;
    /*QImage qrImage = generateAlipayQRCode(strQrCode);
    if (!qrImage.isNull())
    {
        QDir dir;
        GlobalData::strQrcode = dir.tempPath() + "/alipay_qrcode.png";
        qrImage.save(GlobalData::strQrcode);
        int width = ui->labelQrCode->width();
        int height = ui->labelQrCode->height();
        ui->labelQrCode->setPixmap(QPixmap(GlobalData::strQrcode).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        //正常情况
        ui->stackedWidget->setCurrentWidget(ui->pageQrCode);
    }*/
    int width = ui->label->width();
    int height = ui->label->height();
    QString strUrl = ":/main/resource/main/defaultSceenShot.png"; //"https://zcpreview.armvm.com/screenshot/d9f9737fdcf0435faf2b32710ccb5ae4.png";
    ui->label->setPixmap(QPixmap(strUrl).scaled(QSize(ITEM_PHONE_VERTICAL_WIDTH, ITEM_PHONE_VERTICAL_HEIGHT), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    // 使用QNetworkAccessManager获取网络图片
   /* QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(sPhoneInfo.strUrl));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &PhoneItemWidget::do_DownloadImageFinish);
    qDebug() << "download image No=" << sTaskInfo.strPadCode << "Url=" << sTaskInfo.strUrl;*/
    /*m_bDownload = false;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &ItemWidget::do_timeout);
    m_pTimer->start(2000);//2秒间隔
    */
}

PhoneItemWidget::~PhoneItemWidget()
{
    qDebug()<<"delete PhoneItemWidget";
    delete ui;
}

void PhoneItemWidget::do_DownloadImageFinish()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (reply != NULL)
    {
        qDebug() << "do_DownloadImageFinish No=" << m_sTaskInfo.strPadCode;
        QPixmap pixmap;
        this->byteArrayImageUrl = reply->readAll();
        //reply->deleteLater();
        // 将网络图片数据设置到QPixmap对象上
        pixmap.loadFromData(this->byteArrayImageUrl, "PNG");
        //保存到本地查看
        pixmap.save(m_strPicturePath,"png");
        // 将网络图片设置到QLabel上
        ui->label->setPixmap(pixmap.scaled(ITEM_PHONE_VERTICAL_WIDTH, ITEM_PHONE_VERTICAL_HEIGHT)); // 缩放为合适的大小
        //m_bDownload = true;
        //m_pTimer->stop();
    }
}
