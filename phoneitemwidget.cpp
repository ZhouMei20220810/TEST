#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QImage>
#include <QTransform>
#include <QMouseEvent>
//#include <QMatrix>

PhoneItemWidget::PhoneItemWidget(S_PHONE_INFO sPhoneInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true); 
    
    m_sPhoneInfo = sPhoneInfo;
    ui->toolBtnName->setText(sPhoneInfo.strName);
    
    m_strPicturePath = GlobalData::strFileTempDir + sPhoneInfo.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir + sPhoneInfo.strInstanceNo + "_bak.png";

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
    m_checkBox->setChecked(sPhoneInfo.bChecked);
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

    m_refreshTimer = new QTimer();
    connect(m_refreshTimer, &QTimer::timeout, this, [this]()
        {
            m_refreshTimer->stop();
            qDebug() << "init m_strPicturePath" << m_strPicturePath;
            showLabelImage(m_strPicturePath);

        });
    m_refreshTimer->start(1);// 1ms触发一次
}

void PhoneItemWidget::setPhoneName(QString strPhoneName)
{
    ui->toolBtnName->setText(strPhoneName);
}

void PhoneItemWidget::showLabelImage(QString strImagePath)
{
    QFile file1(m_strPicturePath);
    if (file1.exists())
    {        
        qDebug() << "showLabelImage file 存在";
        QPixmap pixmap(m_strPicturePath);
        if (!GlobalData::bVerticalScreen)
        {
            QImage image(m_strPicturePath);
            if (!image.isNull())
            {
                QTransform transform;
                transform.rotate(270);
                image = image.transformed(transform);
                pixmap = QPixmap::fromImage(image);
            }
            else
            {
                qDebug() << "image is null. file=" << m_strPicturePath;
            }
        }        
        if (pixmap.isNull())
        {
            pixmap = QPixmap(m_strPicturePath);
            qDebug() << "pixmap is null. file=" << m_strPicturePath;
        }
        ui->label->setPixmap(pixmap.scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
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
    m_File = new QFile(m_strTemp);
    if (!m_File->open(QIODevice::WriteOnly))
    {
        delete m_File;
        m_File = NULL;
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
    if (m_File)
    {
        m_File->flush();
        //file->close();
        QPixmap pixmap(m_strTemp);
        if (!pixmap.isNull())
        {
            QFile file(m_strPicturePath);
            if (file.exists())
            {
                if (!file.remove())
                {
                    qDebug() << "remove fail:" << m_strPicturePath;
                }                    
            }
            m_File->rename(m_strPicturePath);
            //file.rename(m_strPicturePath);
            qDebug() << "httpFinished m_strPicturePath" << m_strPicturePath;
            showLabelImage(m_strPicturePath);
        }
        else
        {
            if (QFile::exists(m_strPicturePath))
            {
                qDebug() << "httpFinished pixmap is null. m_strPicturePath" << m_strPicturePath;
                showLabelImage(m_strPicturePath);
            }                
            else
            {
                qDebug() << "图片无效,显示默认图片";
                ui->label->setPixmap(QPixmap(":/main/resource/main/defaultSceenShot.png").scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            }
        }
        m_File->close();
        delete m_File;
        m_File = NULL;
    }
    m_reply->deleteLater();
    m_reply = 0;

}

//接受数据中
void PhoneItemWidget::httpReadyRead()
{
    if (m_File) 
    { 
        m_File->write(m_reply->readAll()); 
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
            QMouseEvent* e = static_cast<QMouseEvent*>(event);
            switch (e->button()) 
            {
            case Qt::LeftButton:
            {
                emit ShowPhoneInstanceWidgetSignals(m_sPhoneInfo, false);
                return true;
            }                
                break;
            case Qt::RightButton:
                emit ShowPhoneInstanceWidgetSignals(m_sPhoneInfo,true);
                return true;
            default:
                QWidget::eventFilter(watched, event);
            }
        }        
        else
        {
            return false;
        }
    }

    return QWidget::eventFilter(watched, event);  
}