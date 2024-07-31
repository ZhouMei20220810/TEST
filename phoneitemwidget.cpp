#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QImage>
#include <QTransform>
#include <QMouseEvent>
#include "filedownloader.h"
//#include <QMatrix>

PhoneItemWidget::PhoneItemWidget(S_PHONE_INFO sPhoneInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true); 
    
    m_sPhoneInfo = sPhoneInfo;
    if (sPhoneInfo.strName.isEmpty())
        ui->toolBtnName->setText(sPhoneInfo.strInstanceNo);
    else
        ui->toolBtnName->setText(sPhoneInfo.strName);
    
    m_FileDownload = NULL;
    m_strPicturePath = GlobalData::strFileTempDir + sPhoneInfo.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir + sPhoneInfo.strInstanceNo + "_bak.png";

    m_manager = new QNetworkAccessManager(this);

    m_LabelAccredit = new QLabel(ui->label);
    m_checkBox = new QCheckBox(ui->label);
    m_checkBox->setStyleSheet("QCheckBox{spacing:0px;background:transparent;}QCheckBox::indicator{width: 16px;height: 16px;}QCheckBox::indicator:unchecked{ image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:hover{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:pressed{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:checked{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:hover{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:pressed{image:url(:/login/resource/login/option_select.png);}");    
    m_LabelAccredit->setStyleSheet("QLabel{background:transparent;border:none;}");
    if (sPhoneInfo.bIsAuth)
    {
        //已授权
        m_LabelAccredit->setPixmap(QPixmap(":/main/resource/main/Authorized.png"));
    }
    else if (sPhoneInfo.iAuthType == EN_BE_AUTHORIZATION)
    {
        //被授权
        m_LabelAccredit->setPixmap(QPixmap(":/main/resource/main/BeAuthorized.png"));
    }
    
    QVBoxLayout* vBox = new QVBoxLayout(ui->label);    
    QHBoxLayout* hBox = new QHBoxLayout(ui->label);
    hBox->addWidget(m_LabelAccredit);
    hBox->addStretch();
    hBox->addWidget(m_checkBox);
    hBox->setContentsMargins(0, 0, 0, 0);
    vBox->addLayout(hBox);
    vBox->addStretch();
    vBox->setContentsMargins(0, 8, 8, 0);
    m_checkBox->setCursor(Qt::PointingHandCursor);
    m_checkBox->setChecked(sPhoneInfo.bChecked);
    connect(m_checkBox, &QCheckBox::stateChanged, this, &PhoneItemWidget::stateChanged);
    //未下载时先隐藏进度条
    //ui->progressBar->hide();
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
        if (this->isVisible())
        {
            ui->label->setPixmap(pixmap.scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }        
    }
    else
    {
        QString strDefaultImage = ":/main/resource/main/defaultSceenShot.png";
        QPixmap pixmap(strDefaultImage);
        if (!GlobalData::bVerticalScreen)
        {
            QImage image(strDefaultImage);
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
        if (this->isVisible())
        {
            ui->label->setPixmap(pixmap.scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            qDebug() << "showLabelImage fail. file not exists." << m_strPicturePath;
        }
    }
}

PhoneItemWidget::~PhoneItemWidget()
{
    if (m_FileDownload != NULL && m_FileDownload->isRunning())
    {
        qDebug() << "PhoneItemWidget thread is running";
        m_FileDownload->terminate();
    }
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
void PhoneItemWidget::downloadUrl(QString url)
{
    if(NULL == m_FileDownload)
        m_FileDownload = new FileDownloader(this);
    if (m_FileDownload != NULL)
    {
        connect(m_FileDownload, &FileDownloader::downloadFinished, this, [this]()
            {
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
                if (!QFile::rename(m_strTemp,m_strPicturePath))
                {
                    qDebug() << "rename fail: " << m_strPicturePath;
                }
                //file.rename(m_strPicturePath);
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
            });
        m_FileDownload->setUrlOutputFile(url, m_strTemp);
        m_FileDownload->start();
    }
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
                GlobalData::enRightClickType = EN_ICON_MODE_WIDGET;
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
