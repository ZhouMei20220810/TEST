#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"

PhoneItemWidget::PhoneItemWidget(S_TASK_INFO sTaskInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    m_refreshTimer = new QTimer();
    connect(m_refreshTimer, &QTimer::timeout, this, &PhoneItemWidget::do_timeoutRefreshPicture);
    m_refreshTimer->start(TIMER_INTERVAL);// 每分钟触发一次，60000毫秒
    m_bIsRefresh = true;
    m_sTaskInfo = sTaskInfo;
    
    m_strPicturePath = QDir::tempPath() + "/"+ SCREENSHOT_PICTRUE_FLODER+"/" + sTaskInfo.strPadCode + ".png";
    QFile file1(m_strPicturePath);
    QString strUrl;
    if (!file1.exists())
        strUrl = ":/main/resource/main/defaultSceenShot.png";

    int width = ui->label->width();
    int height = ui->label->height();
    ui->label->setPixmap(QPixmap(strUrl).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    
    m_manager = new QNetworkAccessManager(this);
    //未下载时先隐藏进度条
    //ui->progressBar->hide();

    //初始化文件
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

void PhoneItemWidget::setRefreshTimer(bool bIsRefresh)
{
    m_bIsRefresh = bIsRefresh;
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

//一分钟响应一次
void PhoneItemWidget::do_timeoutRefreshPicture()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    qDebug() << dateTime.toString("yyyy-MM-dd HH:mm:ss");
    if (m_bIsRefresh)
    {
        //请求刷新函数
        qDebug() << "请求刷新函数";
        QPixmap pixmap(m_strPicturePath);
        ui->label->setPixmap(pixmap.scaled(ITEM_PHONE_VERTICAL_WIDTH, ITEM_PHONE_VERTICAL_HEIGHT)); // 缩放为合适的大小
    }
    else
    {
        qDebug() << "停止刷新图片";
        m_refreshTimer->stop();
    }
}