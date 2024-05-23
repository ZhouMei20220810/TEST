#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"

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
    QFile file1(m_strPicturePath);
    QString strUrl;
    if (!file1.exists())
        strUrl = ":/main/resource/main/defaultSceenShot.png";

    int width = ui->label->width();
    int height = ui->label->height();
    ui->label->setPixmap(QPixmap(strUrl).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    /*int width = ui->label->width();
    int height = ui->label->height();

     //"https://zcpreview.armvm.com/screenshot/d9f9737fdcf0435faf2b32710ccb5ae4.png";
    ui->label->setPixmap(QPixmap(strUrl).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    // 使用QNetworkAccessManager获取网络图片
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(sTaskInfo.strUrl));
    QNetworkReply* reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &PhoneItemWidget::do_DownloadImageFinish);
    qDebug() << "download image No="<<sTaskInfo.strPadCode<<"sTaskInfo.strUrl=" << sTaskInfo.strUrl;*/
    /*m_bDownload = false;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &ItemWidget::do_timeout);
    m_pTimer->start(2000);//2秒间隔
    */

    m_manager = new QNetworkAccessManager(this);
    //未下载时先隐藏进度条
    //ui->progressBar->hide();

    //初始化文件
    m_strPicturePath = QDir::tempPath() + "/" + m_sTaskInfo.strPadCode + ".png";
    file = new QFile(m_strPicturePath);
    if (!file->open(QIODevice::WriteOnly))
    {
        delete file;
        file = NULL;
    }

    startRequest(QUrl(m_sTaskInfo.strUrl));

    //进度条清0
    //ui->progressBar->setValue(0);
    //ui->progressBar->show();

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
        qDebug() << "byteArrayImageUrl= " << this->byteArrayImageUrl;
        //reply->deleteLater();        
        // 将网络图片数据设置到QPixmap对象上
        pixmap.loadFromData(this->byteArrayImageUrl, "PNG");
        //保存到本地查看
        //pixmap.save(m_strPicturePath);
        // 将网络图片设置到QLabel上
        ui->label->setPixmap(pixmap.scaled(ITEM_PHONE_VERTICAL_WIDTH, ITEM_PHONE_VERTICAL_HEIGHT)); // 缩放为合适的大小
        pixmap.save(m_strPicturePath);
        //m_bDownload = true;
        //m_pTimer->stop();
        

        reply->deleteLater();
    }
}

void PhoneItemWidget::startRequest(QUrl url)
{
    m_reply = m_manager->get(QNetworkRequest(url));
    connect(m_reply, &QNetworkReply::readyRead, this, &PhoneItemWidget::httpReadyRead);
    connect(m_reply, &QNetworkReply::finished, this, &PhoneItemWidget::httpFinished);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &PhoneItemWidget::updateDataReadProgress);
}

//文件接收完成
void PhoneItemWidget::httpFinished()
{
    if (file)
    {
        file->close();
        QPixmap pixmap(m_strPicturePath);
        ui->label->setPixmap(pixmap.scaled(ITEM_PHONE_VERTICAL_WIDTH, ITEM_PHONE_VERTICAL_HEIGHT)); // 缩放为合适的大小

        delete file;
        file = 0;

        //if(m_strPicturePath)
    }
    m_reply->deleteLater();
    m_reply = 0;

}

//接受数据中
void PhoneItemWidget::httpReadyRead()
{
    if (file) { file->write(m_reply->readAll()); }
}

//进度条更新
void PhoneItemWidget::updateDataReadProgress(qint64 byteRead, qint64 totalBytes)
{
    //ui->progressBar->setMaximum(totalBytes);
    //ui->progressBar->setValue(byteRead);
}