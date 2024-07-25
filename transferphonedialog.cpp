#include "transferphonedialog.h"
#include "ui_transferphonedialog.h"
#include <QGraphicsDropShadowEffect>

TransferPhoneDialog::TransferPhoneDialog(QMap<int, S_PHONE_INFO> mapPhoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransferPhoneDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("转移云机");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    m_mapLevelList = mapLevelList;

    ui->listWidget->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    LoadWidgetData(mapPhoneInfo);
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

    QString strStyleSheet = "QCheckBox{spacing:5px;}QCheckBox::indicator{width:16px;height:16px;}QCheckBox::indicator:unchecked{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:hover{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:pressed{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:checked{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:hover{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:pressed{image:url(:/login/resource/login/option_select.png);}";
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
        if (strLevelName.compare("BVIP", Qt::CaseInsensitive) == 0)
            checkBox->setIcon(QIcon(QString(":/main/resource/main/XVIP.png")));
        else
            checkBox->setIcon(QIcon(QString(":/main/resource/main/%1.png").arg(strLevelName)));
        if (iter->strName.isEmpty())
            checkBox->setText(iter->strInstanceNo);
        else
            checkBox->setText(iter->strName);
        checkBox->setFixedSize(340, LISTMODE_ITEM_HEGITH);
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