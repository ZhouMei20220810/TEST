#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QImage>
#include <QTransform>
//#include <QMatrix>

PhoneItemWidget::PhoneItemWidget(S_PHONE_INFO sTaskInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);

    m_PhoneInstanceWidget = NULL;
    m_refreshTimer = new QTimer();
    connect(m_refreshTimer, &QTimer::timeout, this, [this]() 
    {
        m_refreshTimer->stop();

        QFile file1(m_strPicturePath);
        QString strUrl;
        if (!file1.exists())
            strUrl = ":/main/resource/main/defaultSceenShot.png";
        else
            strUrl = m_strPicturePath;
        QPixmap pixmap;
        QImage image(strUrl);
        if (!image.isNull())
        {
            if (!GlobalData::bVerticalScreen)
            {
                QTransform transform;
                transform.rotate(270);
                image = image.transformed(transform);
            }
        }

        pixmap = QPixmap::fromImage(image);
        if (pixmap.isNull())
        {
            pixmap = QPixmap(strUrl);
        }
        ui->label->setPixmap(pixmap.scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    });
    m_refreshTimer->start(1);// 1ms触发一次
    
    m_sTaskInfo = sTaskInfo;
    ui->toolBtnName->setText(sTaskInfo.strName);
    
    m_strPicturePath = GlobalData::strFileTempDir+"/" + sTaskInfo.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir + "/" + sTaskInfo.strInstanceNo + "_bak.png";   

    m_manager = new QNetworkAccessManager(this);

    m_LabelAccredit = new QLabel(ui->label);
    m_checkBox = new QCheckBox(ui->label);
    m_checkBox->setStyleSheet("QCheckBox{spacing:0px;background:transparent;}QCheckBox::indicator{width: 16px;height: 16px;}QCheckBox::indicator:unchecked{ image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:hover{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:pressed{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:checked{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:hover{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:pressed{image:url(:/login/resource/login/option_select.png);}");    
    m_LabelAccredit->setStyleSheet("QLabel{background:transparent;border:none;}");
    QVBoxLayout* vBox = new QVBoxLayout(ui->label);    
    QHBoxLayout* hBox = new QHBoxLayout(ui->label);
    hBox->addWidget(m_LabelAccredit);
    hBox->addStretch();
    hBox->addWidget(m_checkBox);
    vBox->addLayout(hBox);
    vBox->addStretch();

    //未下载时先隐藏进度条
    //ui->progressBar->hide();

    //初始化文件
    /*file = new QFile(m_strPicturePath);
    if (!file->open(QIODevice::WriteOnly))
    {
        delete file;
        file = NULL;
    }*/

    //startRequest(QUrl(m_sTaskInfo.strUrl));

    //进度条清0
    //ui->progressBar->setValue(0);
    //ui->progressBar->show();

    ui->label->installEventFilter(this);
}

PhoneItemWidget::~PhoneItemWidget()
{
    qDebug()<<"delete PhoneItemWidget";
    delete ui;
}

void PhoneItemWidget::setCheckBoxStatus(bool bCheck)
{
    m_checkBox->setChecked(bCheck);
}
bool PhoneItemWidget::getCheckBoxStatus()
{
    return m_checkBox->isChecked();
}
void PhoneItemWidget::startRequest(QUrl url)
{
    //初始化文件
    file = new QFile(m_strTemp);
    if (!file->open(QIODevice::WriteOnly))
    {
        delete file;
        file = NULL;
    }
    qDebug() << "PhoneItemWidget::startRequest url=" << url << " file=" << m_strTemp;
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
        QPixmap pixmap(m_strTemp);
        if (!pixmap.isNull())
        {
            if (QFile::exists(m_strPicturePath))
            {
                if (!QFile::remove(m_strPicturePath))
                {
                    qDebug() << "remove fail:" << m_strPicturePath;
                }                    
            }
            file->rename(m_strPicturePath);
            pixmap = QPixmap(m_strPicturePath);
            ui->label->setPixmap(pixmap.scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            qDebug() << "图片无效,显示默认图片";
            if(QFile::exists(m_strPicturePath))
                ui->label->setPixmap(QPixmap(m_strPicturePath).scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            else
                ui->label->setPixmap(QPixmap(":/main/resource/main/defaultSceenShot.png").scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        delete file;
        file = 0;
    }
    m_reply->deleteLater();
    m_reply = 0;

}

//接受数据中
void PhoneItemWidget::httpReadyRead()
{
    if (file) 
    { 
        file->write(m_reply->readAll()); 
    }
}

//进度条更新
void PhoneItemWidget::updateDataReadProgress(qint64 byteRead, qint64 totalBytes)
{
    //ui->progressBar->setMaximum(totalBytes);
    //ui->progressBar->setValue(byteRead);
}

bool PhoneItemWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->label)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
            //m_enQuality = (ENUM_PICTURE_QUALITY)setting.value("PictureQuality", TYPE_QUALITY_HIGH_SPEED).toInt();
            GlobalData::bVerticalPhoneInstance = setting.value("VerticalScreen", true).toBool();
            GlobalData::bVerticalPhoneInstanceCenter = setting.value("PhoneInstanceCenter", true).toBool();
            if (NULL == m_PhoneInstanceWidget)
            {
                m_PhoneInstanceWidget = new PhoneInstanceWidget(m_sTaskInfo);
            }
            if (!GlobalData::bVerticalPhoneInstanceCenter)
            {
                QPoint point = setting.value("PhoneInstancePoint").toPoint();
                m_PhoneInstanceWidget->move(point);
            }           

            m_PhoneInstanceWidget->show();
            connect(m_PhoneInstanceWidget, &PhoneInstanceWidget::destroyed, this, [this]() {
                m_PhoneInstanceWidget = NULL;
                });
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(watched, event);
    }    
}