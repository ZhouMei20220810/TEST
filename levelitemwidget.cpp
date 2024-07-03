#include "levelitemwidget.h"
#include "ui_levelitemwidget.h"
#include "global.h"
#include "filedownloader.h"

LevelItemWidget::LevelItemWidget(S_LEVEL_INFO levelInfo, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LevelItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QSize size(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);
    this->resize(size);

    m_levelInfo = levelInfo;

    m_toolBtn = new QToolButton(this);
    QString strIcon = QString(":/main/resource/buy/%1_level_bg_normal.png").arg(levelInfo.strLevelName);
    //m_toolBtn->setIcon(QIcon(":/main/resource/main/gvip_bg_normal.png"));
    m_toolBtn->resize(size);
    m_toolBtn->setIcon(QIcon(strIcon));
    m_toolBtn->setIconSize(QSize(ITEM_WIDGET_LEVEL_WIDTH - 2, ITEM_WIDGET_LEVEL_HEIGHT));
    connect(m_toolBtn, &QToolButton::clicked, this, &LevelItemWidget::on_toolButtonBG_clicked);

    ui->labelVersion->setParent(m_toolBtn);
    ui->labelFunction->setParent(m_toolBtn);
    ui->frame->setParent(m_toolBtn);

    QVBoxLayout* vBox = new QVBoxLayout();
    vBox->addWidget(ui->labelVersion);
    vBox->addWidget(ui->labelFunction);
    vBox->addWidget(ui->frame);
    vBox->setContentsMargins(24,24,24,24);
    m_toolBtn->setLayout(vBox);

    ui->labelVersion->setText(levelInfo.strLevelName);
    ui->labelFunction->setText(levelInfo.strLevelRemark);

    QString url = levelInfo.strColorIcon;
    QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
    
    QString strTmp = GlobalData::strPictureTempDir + strFileName;
    qDebug() << "url=" << url << "strTmp=" << strTmp;
    //ui->labelVersion->setPixmap(QPixmap(strTmp));
    startDownload(url);
    int width = ui->labelVersion->width();
    int height = ui->labelVersion->height();
    QFile file(strTmp);
    if(file.exists())
        ui->labelVersion->setPixmap(QPixmap(strTmp).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

LevelItemWidget::~LevelItemWidget()
{
    qDebug()<<"delete LevelItemWidget";
    delete ui;
}

void LevelItemWidget::startDownload(QString strUrl)
{
    FileDownloader* downloader = new FileDownloader(this);
    connect(downloader, &FileDownloader::downloadFinished, this, [this](bool success, QString errorMessage)
        {
            if (success)
            {
                QString url = m_levelInfo.strColorIcon;
                QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
                QString strTmp = GlobalData::strPictureTempDir + strFileName;
                qDebug() << "url=" << url << "strTmp=" << strTmp;
                //ui->labelVersion->setPixmap(QPixmap(strTmp));
                //startDownload(url);
                int width = ui->labelVersion->width();
                int height = ui->labelVersion->height();
                QFile file(strTmp);
                if (file.exists())
                {
                    ui->labelVersion->setPixmap(QPixmap(strTmp).scaled(QSize(width, height), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }
            }
            else
            {
                qDebug() <<"download="<< m_levelInfo.strColorIcon<< "errorMessage = " << errorMessage;
            }
                            
        });
    downloader->setUrl(strUrl, "xxx.png");
    downloader->start();
}

S_LEVEL_INFO LevelItemWidget::getLevelInfo()
{
    return m_levelInfo;
}

void LevelItemWidget::setLabelCheckStatus(bool bCheck)
{
    QString strIcon;
    if (bCheck)
    {
        strIcon = QString(":/main/resource/buy/%1_level_bg_select.png").arg(m_levelInfo.strLevelName);
    }
    else
    {
        strIcon = QString(":/main/resource/buy/%1_level_bg_normal.png").arg(m_levelInfo.strLevelName);
    }
    m_toolBtn->setIcon(QIcon(strIcon));

    qDebug()<<"LevelItemWidget labelCheck status="<<bCheck;
}

void LevelItemWidget::on_toolButtonBG_clicked()
{
    QString strIcon = QString(":/main/resource/buy/%1_level_bg_select.png").arg(m_levelInfo.strLevelName);
    m_toolBtn->setIcon(QIcon(strIcon));
    
    emit selectLevelTypeSignals(m_levelInfo);
}

