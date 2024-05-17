#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTreeWidget>
#include <QMenu>
#include "creategroupwidget.h"
#include "updategroupwidget.h"
//#include "levelitemwidget.h"
#include <QAbstractItemView>
#include "vipitemwidget.h"
#include "Logoutdialog.h"
#include "messagetipsdialog.h"
#include "phoneitemwidget.h"
#include <QScrollBar>
#include <QFile>
#include <QDir>
#include "paywidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose,true);
	setAttribute(Qt::WA_Hover, true);

    //初始化Tab云手机
    InitCloudPhoneTab();
    //初始化Tab激活码
    InitActiveCodeTab();
    //初始化Tab购买
    InitBuyTab();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//云手机
void MainWindow::InitCloudPhoneTab()
{
    //设置TreeWidget相关属性
    //ui->treeWidget->resize(200, 600);
    //ui->treeWidget->move(100, 80);

    ui->treeWidget->setColumnCount(1);
    //ui->treeWidget->setHeaderLabel("分组列表");
    //ui->treeWidget->setHeaderLabels("分组列表");
    //隐藏表头
    ui->treeWidget->setHeaderHidden(true);

    //设置复选框
    ui->treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    //ui->treeWidget->setCheckBoxes(true);

    //隐藏续费列表
    ui->listWidgetRenew->setHidden(true);

    //隐藏展开按钮
    ui->btnExpansion->setVisible(false);
    //隐藏展开功能按钮
    ui->toolBtnExpansionFunction->setVisible(false);

    //初始化设备列表
    InitPhoneList();

    //初始化QComboBox
    ui->comboBoxSystem->addItem("安卓","andrios");
    ui->comboBoxSystem->addItem("苹果","ios");

    ui->comboBoxLevel->addItem("标准型","vip");
    ui->comboBoxLevel->addItem("增强型","GVIP");
    ui->comboBoxLevel->addItem("尊享型","KVIP");

    //隐藏切换竖屏按钮
    ui->toolBtnChangeVerScreen->setVisible(false);

}


//激活码
void MainWindow::InitActiveCodeTab()
{

}

//购买
void MainWindow::InitBuyTab()
{
    InitLevelList();
    InitVipList();

    QStringList stringList;
    stringList<<"等级/名称"<<"剩余时间";
    ui->listWidgetRenewList->addItems(stringList);

    //隐藏续费列表
    ui->listWidgetRenewList->setHidden(true);
    ui->listWidgetRenewList->setModelColumn(2);
    ui->listWidgetRenewList->setSelectionMode(QAbstractItemView::SingleSelection);
    QLabel* label=new QLabel(this);
    label->setText("等级/名称");
    label->setStyleSheet("background-color:gray;color:#FF2323FF");
    QListWidgetItem* item = new QListWidgetItem(ui->listWidgetRenewList);
    ui->listWidgetRenewList->setItemWidget(item,label);
    ui->listWidgetRenewList->addItem(item);


    label=new QLabel(this);
    label->setText("剩余时间");
    label->setStyleSheet("background-color:gray;color:#FF2323FF");
    item = new QListWidgetItem(ui->listWidgetRenewList);
    ui->listWidgetRenewList->setItemWidget(item,label);
    ui->listWidgetRenewList->addItem(item);

    //默认显示
    ui->stackedWidget_2->setCurrentWidget(ui->page_Meal);
}

//初始化列表
void MainWindow::InitPhoneList()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    //默认显示竖屏
    on_toolBtnChangeVerScreen_clicked();
}

void MainWindow::InitLevelList()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    /*ui->listWidgetLevel->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetLevel->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    //ui->listWidgetLevel->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetLevel->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetLevel->setSelectionMode(QAbstractItemView::SingleSelection);

    //初始化level listwidget
    //ui->listWidgetLevel->setFixedHeight(254);
    ui->listWidgetLevel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidgetLevel->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //ui->listWidgetLevel->setFixedSize(QSize())
    ui->listWidgetLevel->verticalScrollBar()->setVisible(false);
    ui->listWidgetLevel->setFixedWidth(3*(ITEM_WIDGET_LEVEL_WIDTH+15));*/

    //初始化列表云手机购买列表+云手机续费 等级列表
    //LevelItemWidget* widget = NULL;
    //QListWidgetItem* item = NULL;
    //int iCount = map.size();
    //qDebug() << "map size() =" << iCount;
    //QMap<int, S_ItemWidgetData*>::const_iterator iter = map.constBegin();
    //for (; iter != map.constEnd(); iter++)
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QToolButton* toolBtn;
    QString strImage;
    //QWidget* containerWidget = new QWidget();
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    //containerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QHBoxLayout* horizontalLayout = new QHBoxLayout(ui->scrollAreaWidgetContents);
    horizontalLayout->setSpacing(50);

    normal = new LevelNormalWidget(LEVEL_NOMAL_LEVEL, ui->scrollArea);
    connect(normal, &LevelNormalWidget::selectLevelTypeSignals, this, &MainWindow::do_selectLevelTypeSignals);
    horizontalLayout->addWidget(normal);

    enhancement = new LevelEnhancementWidget(LEVEL_ENHANCEMENT_TYPE, ui->scrollArea);
    connect(enhancement, &LevelEnhancementWidget::selectLevelTypeSignals, this, &MainWindow::do_selectLevelTypeSignals);
    horizontalLayout->addWidget(enhancement);

    premier = new LevelPremierWidget(LEVEL_PREMIER_TYPE, ui->scrollArea);
    connect(premier, &LevelPremierWidget::selectLevelTypeSignals, this, &MainWindow::do_selectLevelTypeSignals);
    horizontalLayout->addWidget(premier);
}
void MainWindow::InitVipList()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidgetVIP->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetVIP->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetVIP->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetVIP->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetVIP->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::InitVipRenewList()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidgetRenewList->setViewMode(QListView::ListMode);//(QListView::IconMode);

    //ui-->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetRenewList->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetRenewList->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetRenewList->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetRenewList->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList headers;
    headers << "Column 1" << "Column 2" << "Column 3";
    //ui->listWidgetRenewList->setHeaderLabels(headers);
}

void MainWindow::QueryAllGroup()//查询全部分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_QUERY_ALL_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
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
                qDebug() << "Code=" << iCode << "message=" << strMessage << "response:" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        int iGroupSize = dataArray.size();
                        if(0 == iGroupSize)
                        {
                            return;
                        }
                        else
                        {
                            m_mapGroupInfo.clear();
                            ui->treeWidget->clear();
                            //树形列表显示
                            S_GROUP_INFO groupInfo;
                            QJsonObject dataObj;
                            for(int i= 0; i < iGroupSize; i++)
                            {
                                dataObj = dataArray[i].toObject();
                                groupInfo.iGroupId = dataObj["id"].toInt();
                                groupInfo.iGroupNum = dataObj["num"].toInt();
                                groupInfo.strGroupName = dataObj["name"].toString();
                                qDebug() << "iGroupId="<< groupInfo.iGroupId <<"strGroupName="<< groupInfo.strGroupName <<"iGroupCount="<< groupInfo.iGroupNum;

                                m_mapGroupInfo.insert(i,groupInfo);
                            }

                            //调用UI接口显示数据
                            ShowGroupInfo();
                        }
                    }
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::ShowGroupInfo()
{
    if(m_mapGroupInfo.size() <= 0)
        return;

    QTreeWidgetItem* item = NULL;
    QTreeWidgetItem* child = NULL;
    QMap<int, S_GROUP_INFO>::iterator iter = m_mapGroupInfo.begin();
    for( ; iter != m_mapGroupInfo.end(); iter++)
    {
        item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0,iter->strGroupName);
        //存储GroupId
        item->setData(0, Qt::UserRole, iter->iGroupId);
        ui->treeWidget->addTopLevelItem(item);

        //添加子节点
        child = new QTreeWidgetItem(item);
        child->setText(0, iter->strGroupName + "子节点");
        //将节点添加到根节点下
        item->addChild(child);

        child = new QTreeWidgetItem(item);
        child->setText(0, iter->strGroupName + "子节点1");
        //将节点插入到第一个位置
        item->addChild(child);
        //同上
        ui->treeWidget->insertTopLevelItem(0, child);
    }

    //展开所有
    ui->treeWidget->expandAll();
}

void MainWindow::CreateGroup(QString strGroupName)//创建分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("name", strGroupName);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
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
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json="<<response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //界面添加该组
                    QueryAllGroup();
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::UpdateGroup(int iGroupId, QString strNewName)//修改分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_UPDATE_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("createBy", "");
    obj.insert("id", iGroupId);
    obj.insert("name", strNewName);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
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
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功
                    if (data)
                    {
                        //界面直接修改名称不需要重新请求
                        QueryAllGroup();
                    }
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::DeleteGroup(int iGroupId)//删除分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_GROUP;
    strUrl += QString("/%1").arg(iGroupId);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功
                    if (data)
                    {
                        //界面直接修改名称不需要重新请求
                        QueryAllGroup();
                    }
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//获取serverToken
/*void MainWindow::HttpGetServerToken()
{
    int iGroupId = 0;
    QTreeWidgetItem* selectItem = ui->treeWidget->currentItem();
    if (selectItem != NULL)
    {
        iGroupId = selectItem->data(0, Qt::UserRole).toInt();
    }

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_SERVER_TOKEN;
    strUrl += QString("/%1").arg(iGroupId);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    qDebug() << "操作成功";
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}*/

// 订单接口-我的支付订单
void MainWindow::HttpGetMyOrder(int iPage,int iPageSize)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_ORDER;
    strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
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
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();
                        int iSize = data["size"].toInt();
                        int iTotal = data["total"].toInt();
                        int iMaxLimit = data["maxLimit"].toInt();
                        int iCountId = data["countId"].toInt();
                        bool bSearchCount = data["searchCount"].toBool();
                        bool bOptimizeCountSql = data["optimizeCountSql"].toBool();

                        QJsonArray records = data["records"].toArray();
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            /*QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_PHONE_INFO phoneInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                phoneInfo.strCreateTime = recordObj["createTime"].toString();
                                phoneInfo.strCurrentTime = recordObj["current"].toString();
                                phoneInfo.strExpireTime = recordObj["expireTime"].toString();
                                phoneInfo.iId = recordObj["id"].toInt();
                                phoneInfo.iLevel = recordObj["level"].toInt();
                                phoneInfo.strName = recordObj["name"].toString();
                                phoneInfo.strInstanceNo = recordObj["no"].toString();
                                phoneInfo.strServerToken = recordObj["serverToken"].toString();
                                phoneInfo.iType = recordObj["type"].toInt();
                            }*/
                        }
                        QJsonArray orders = data["orders"].toArray();
                        if (orders.size() > 0)
                        {
                            int iOrdersSize = orders.size();
                            /*QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_PHONE_INFO phoneInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                phoneInfo.strCreateTime = recordObj["createTime"].toString();
                                phoneInfo.strCurrentTime = recordObj["current"].toString();
                                phoneInfo.strExpireTime = recordObj["expireTime"].toString();
                                phoneInfo.iId = recordObj["id"].toInt();
                                phoneInfo.iLevel = recordObj["level"].toInt();
                                phoneInfo.strName = recordObj["name"].toString();
                                phoneInfo.strInstanceNo = recordObj["no"].toString();
                                phoneInfo.strServerToken = recordObj["serverToken"].toString();
                                phoneInfo.iType = recordObj["type"].toInt();
                            }*/
                        }
                    }
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//订单接口-创建订单
void MainWindow::HttpCreateOrder(int iChannel,int iMemberId,int iNum, int iPayType,QString strRelateId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_ORDER;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("channel", iChannel);
    obj.insert("memberId", iMemberId);
    obj.insert("num", iNum);
    obj.insert("payType", iPayType);
    //obj.insert("relateId", strRelateId);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() <<"response="<< response;

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
                qDebug() << "Code=" << iCode << "message=" << strMessage;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    QString strData = obj["data"].toString();
                    qDebug() << strData;

                    QDir dir;
                    QString strFilePath = dir.tempPath() + "/" + OPEN_ZHIFUBAO_TEMP_FILE_NAME;
                    /*QFile file(strFilePath);
                    if (file.exists())
                        file.remove();
                    if (file.open(QIODevice::ReadWrite))
                    {
                        QDataStream stream(&file);
                        stream << strData;
                        file.flush();
                        file.close();                        
                    }*/

                    PayWidget* pay = new PayWidget(strFilePath);
                    pay->show();
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//关闭订单
void MainWindow::HttpCloseOrder(QString strOutTradeNo)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += "/api/order/closeOrder";// HTTP_CLOSE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(strOutTradeNo);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                bool bData = obj["data"].toBool();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if(bData)
                        qDebug() << "关闭订单成功";
                    else
                        qDebug() << "关闭订单失败";
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//删除
void MainWindow::HttpDeleteOrder(int iOrderId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(iOrderId);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                bool bData = obj["data"].toBool();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (bData)
                        qDebug() << "删除订单成功";
                    else
                        qDebug() << "删除订单失败";
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//清空
void MainWindow::HttpEmptyOrder()
{

}

//获取我的手机实例
void MainWindow::GetMyPhoneInstance()
{
    int iGroupId = 0;
    int iLevel = 0;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_PHONE_INSTANCE;
    strUrl += QString("?groupId=0&level=0&page=1&pageSize=10");
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
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
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();

                        QJsonArray records = data["records"].toArray();
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_PHONE_INFO phoneInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                phoneInfo.strCreateTime = recordObj["createTime"].toString();
                                phoneInfo.strCurrentTime = recordObj["current"].toString();
                                phoneInfo.strExpireTime = recordObj["expireTime"].toString();
                                phoneInfo.iId = recordObj["id"].toInt();
                                phoneInfo.iLevel = recordObj["level"].toInt();
                                phoneInfo.strName = recordObj["name"].toString();
                                phoneInfo.strInstanceNo = recordObj["no"].toString();
                                phoneInfo.strServerToken = recordObj["serverToken"].toString();
                                phoneInfo.iType = recordObj["type"].toInt();
                            }
                        }
                    }
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//云手机
void MainWindow::on_toolBtnCloudPhone_clicked()
{
    if(0 == ui->stackedWidget->currentIndex())
    {
        return;
    }
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/cloudPhone_select.png);}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/activeCode_normal.png);}");
    ui->toolBtnBuy->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/buy_normal.png);}");


    //ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->stackedWidget->setCurrentIndex(0);

}

//激活码
void MainWindow::on_toolBtnActiveCode_clicked()
{
    if(1 == ui->stackedWidget->currentIndex())
    {
        return;
    }
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/cloudPhone_normal.png);}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/activeCode_select.png);}");
    ui->toolBtnBuy->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/buy_normal.png);}");
    //ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->stackedWidget->setCurrentIndex(1);
}

//购买
void MainWindow::on_toolBtnBuy_clicked()
{
    if(2 == ui->stackedWidget->currentIndex())
    {
        return;
    }
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/cloudPhone_normal.png);}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/activeCode_normal.png);}");
    ui->toolBtnBuy->setStyleSheet("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/buy_select.png);}");
    //ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::setUserInfo(S_USER_LOGIN_INFO userInfo)
{
    m_userInfo = userInfo;
    ui->labelAccount->setText(m_userInfo.strAccount);
    qDebug() << "跳转到主页面" << "id=" << userInfo.id << "name=" << userInfo.strName << "account=" << userInfo.strAccount << "mobile=" << userInfo.strMobile << "MaxExpirationDate" << userInfo.strMaxExpirationDate << "token=" << userInfo.strToken;
    QueryAllGroup();
}

//菜单栏
void MainWindow::on_btnPhotoUrl_clicked()
{

}


void MainWindow::on_btnCustomerService_clicked()
{

}


void MainWindow::on_btnSetting_clicked()
{

}


void MainWindow::on_btnMin_clicked()
{
    this->showMinimized();
}


void MainWindow::on_btnMax_clicked()
{
    this->showMaximized();
}


void MainWindow::on_btnClose_clicked()
{
    LogoutDialog *logoutDialog = new LogoutDialog();
    int ret = logoutDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    //关闭窗口并且退出登录
    qDebug() << "注销";
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_LOGOUT;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;

    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished,this, [=] {
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
                qDebug() << "Code=" << iCode << "message=" << strMessage <<"response:"<<response;
                if(HTTP_SUCCESS_CODE == iCode)
                {
                    /*if (obj["data"].isObject())
                        {
                            QJsonObject data = obj["data"].toObject();
                            QString strToken = data["token"].toString();
                            QString strMaxExpirationDate = data["maxExpirationDate"].toString();

                            QJsonObject userDetailVO = data["userDetailVO"].toObject();
                            int id = userDetailVO["id"].toInt();
                            QString strName = userDetailVO["name"].toString();
                            QString strAccount = userDetailVO["account"].toString();
                            QString strMobile = userDetailVO["mobile"].toString();
                            qDebug() << "跳转到主页面"<<"id="<<id<<"name="<<strName<<"account="<<strAccount<<"mobile="<<strMobile<<"MaxExpirationDate"<<strMaxExpirationDate<<"token="<<strToken;
                        }*/
                    qDebug()<<"注销成功";
                    this->close();
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });    
}


void MainWindow::on_btnCreateGroup_clicked()
{
    //新建分组
    /*CreateGroupWidget* createGroupWidget = new CreateGroupWidget();
    connect(createGroupWidget, &CreateGroupWidget::createGroupSignals, this,[this](QString strGroupName)
            {
        //创建分组
        qDebug()<<" 创建分组 strGroupName="<<strGroupName;
        CreateGroup(strGroupName);
    });
    createGroupWidget->show();*/


    //调试修改分组接口
    //UpdateGroup(2, "新名称");

    //调试删除分组接口
    //DeleteGroup(0);

    //调试获取serverToken接口
    GetMyPhoneInstance();
}

void MainWindow::on_btnGroupRefresh_clicked()
{
    qDebug()<<"刷新";
    //重新加载列表
    QueryAllGroup();

    //测试获取SeverToken接口
    //GetMyPhoneInstance();
}


void MainWindow::on_btnActiveCode_clicked()
{
    QString strActiveCode = ui->lineEditActiveCode->text();
    qDebug()<<"点击激活" << strActiveCode;
}


void MainWindow::on_toolBtnAdd_clicked()
{
    //新增云手机激活
    ui->toolButtonRenew->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnAdd->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->listWidgetRenew->setHidden(true);
}


void MainWindow::on_toolButtonRenew_clicked()
{
    ui->toolBtnAdd->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolButtonRenew->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    //云手机续时激活
    ui->listWidgetRenew->setHidden(false);
}


void MainWindow::on_toolBtnBuyPhone_clicked()
{
    ui->toolBtnBuyPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);background-color:#FF9092A4;border-radius:1px;padding-left:8px;}QToolButton:hover{color:rgb(255,255,255);background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnRenewPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: rgb(255, 255, 255);border-radius:1px;padding-left:8px;}");
    ui->listWidgetRenewList->setHidden(true);
}


void MainWindow::on_toolBtnRenewPhone_clicked()
{
    ui->toolBtnRenewPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);background-color:#FF9092A4;border-radius:1px;padding-left:8px;}QToolButton:hover{color:rgb(255,255,255);background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnBuyPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: rgb(255, 255, 255);border-radius:1px;padding-left:8px;}");
    ui->listWidgetRenewList->setHidden(false);
}


void MainWindow::on_btnBeginPay_clicked()
{
    //确定支付
    bool bAgree = ui->checkBox->isChecked();
    if(!bAgree)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请勾选协议!",this);
        tips->show();
        return;
    }

	//调试传参
    HttpCreateOrder(1, 1, 1, 1, "");

    //HttpGetMyOrder(1, 10);
}

//level item 
void MainWindow::do_selectLevelTypeSignals(LEVEL_TYPE enType)
{
    qDebug() << "click do_selectLevelTypeSignals level Type="<<enType;
    switch (enType)
    {
    case LEVEL_NOMAL_LEVEL:
    {
        enhancement->setLabelCheckStatus(false);
        premier->setLabelCheckStatus(false);
    }
        break;
    case LEVEL_ENHANCEMENT_TYPE:
    {
        normal->setLabelCheckStatus(false);
        premier->setLabelCheckStatus(false);
    }
        break;
    case LEVEL_PREMIER_TYPE:
    {
        normal->setLabelCheckStatus(false);
        enhancement->setLabelCheckStatus(false);
    }
        break;
    default:
        break;
    }
    //设置显示
    /*LevelItemWidget* levelItemWidget = NULL;
    QListWidgetItem* levelItem=NULL;
    LEVEL_TYPE currentType;
    int iCount = ui->listWidgetLevel->count();    
    for(int iRow=0; iRow < iCount;iRow++)
    {
        levelItem = ui->listWidgetLevel->item(iRow);
        currentType = (LEVEL_TYPE)levelItem->data(Qt::UserRole).toInt();
        if (currentType != enType)
        {
            levelItemWidget = static_cast<LevelItemWidget*>(ui->listWidgetLevel->itemWidget(levelItem));
            //enType = (LEVEL_TYPE)item->data(Qt::UserRole).toInt();
            qDebug()<<"iRow="<<iRow<<"currentType="<<currentType<<"选中状态：";
            levelItemWidget->setLabelCheckStatus(false);

            //续费的列表
            //ui->listWidgetRenewList
        }
    }*/

    //加载vip列表
    loadVipType(enType);
}

//初始化vip列表
void MainWindow::loadVipType(LEVEL_TYPE enType)
{
    QString strLevelTypeText = getLevelTypeToText(enType);
    ui->label_2->setText(QString("%1套餐").arg(strLevelTypeText));
    //清空列表
    ui->listWidgetVIP->clear();

    //假设暂时没有KVIP的数据
    if(enType == LEVEL_PREMIER_TYPE)
    {
        qDebug()<<"无套餐";
        ui->stackedWidget_2->setCurrentWidget(ui->page_EmptyMeal);
        return;
    }

    qDebug() << "加载vip列表 enType=" << enType;
    //加载套餐列表
    //ui->listWidgetVIP
    ui->stackedWidget_2->setCurrentWidget(ui->page_Meal);
    int iVIPType = 0;
    QListWidgetItem* vipItem = NULL;
    VIPItemWidget* vipWidget = NULL;
    S_VIP_ITEM_INFO sVipInfo;
    ui->widget->setVisible(true);
    for (int iVIPType = 0; iVIPType < ITEM_WIDGET_VIP_COUNT; iVIPType++)
    {
        vipItem = new QListWidgetItem(ui->listWidgetVIP);
        vipItem->setSizeHint(QSize(ITEM_WIDGET_VIP_WIDTH, ITEM_WIDGET_VIP_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        vipItem->setData(Qt::UserRole, iVIPType);
        ui->listWidgetVIP->addItem(vipItem);

        qDebug() << "vip=" << iVIPType;
        sVipInfo.iDayCount = 30;
        sVipInfo.vipType = (VIP_TYPE)iVIPType;
        sVipInfo.fTotalPrice = 58.80f+iVIPType*100;
        sVipInfo.fDayPrice = sVipInfo.fTotalPrice / sVipInfo.iDayCount;
        sVipInfo.strVipText = strLevelTypeText;
        vipWidget = new VIPItemWidget(sVipInfo, this);
        connect(vipWidget, &VIPItemWidget::selectVIPTypeSignals, this, &MainWindow::do_selectVIPTypeSignals);
        ui->listWidgetVIP->setItemWidget(vipItem, vipWidget);
    }
}

//vip item
void MainWindow::do_selectVIPTypeSignals(S_VIP_ITEM_INFO sVipInfo)
{
    m_curVIPInfo = sVipInfo;
    qDebug() << "click do_selectVIPTypeSignals vip Type=" << sVipInfo.vipType;
    QString str;
    str = str.asprintf("%.2f", sVipInfo.fTotalPrice);
    ui->labelPayMoney->setText(str);

    //设置显示
    VIPItemWidget* vipItemWidget = NULL;
    QListWidgetItem* vipItem = NULL;
    VIP_TYPE curType;
    int iCount = ui->listWidgetVIP->count();
    for (int iRow = 0; iRow < iCount; iRow++)
    {
        vipItem = ui->listWidgetVIP->item(iRow);
        curType = (VIP_TYPE)vipItem->data(Qt::UserRole).toInt();
        if (curType != sVipInfo.vipType)
        {
            vipItemWidget = static_cast<VIPItemWidget*>(ui->listWidgetVIP->itemWidget(vipItem));
            //enType = (LEVEL_TYPE)item->data(Qt::UserRole).toInt();
            qDebug() << "当前选中：等级" << curType;
            vipItemWidget->setLabelCheckStatus(false);

            //续费的列表
            //ui->listWidgetRenewList
        }
    }
}

void MainWindow::on_btnDecrese_clicked()
{
    //减少
    QString strBuyNum = ui->lineEditBuyNumber->text();
    int iBuyNum = strBuyNum.toInt();
    if(iBuyNum > 1)
        strBuyNum = strBuyNum.asprintf("%d",--iBuyNum);
    ui->lineEditBuyNumber->setText(strBuyNum);
}


void MainWindow::on_btnAdd_clicked()
{
    //增加
    QString strBuyNum = ui->lineEditBuyNumber->text();
    int iBuyNum = strBuyNum.toInt();
    strBuyNum = strBuyNum.asprintf("%d",++iBuyNum);
    ui->lineEditBuyNumber->setText(strBuyNum);
}


void MainWindow::on_lineEditBuyNumber_textChanged(const QString &arg1)
{
    //文本框值改变
    int iBuyNum = arg1.toInt();//ui->lineEditBuyNumber->text().toInt();
    QString str;
    str=str.asprintf("%.2f", iBuyNum* m_curVIPInfo.fTotalPrice);
    ui->labelPayMoney->setText(str);
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos()-frameGeometry().topLeft();
        raise();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::on_btnContraction_clicked()
{
    ui->btnContraction->setVisible(false);
    ui->btnExpansion->setVisible(true);

    ui->groupWidget->setVisible(false);
}


void MainWindow::on_btnExpansion_clicked()
{
    ui->btnContraction->setVisible(true);
    ui->btnExpansion->setVisible(false);

    ui->groupWidget->setVisible(true);
}


void MainWindow::on_toolBtnDropFunction_clicked()
{
    //收起功能
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);
    ui->frame_8->setVisible(false);

    ui->toolBtnDropFunction->setVisible(false);
    ui->toolBtnExpansionFunction->setVisible(true);
}


void MainWindow::on_toolBtnExpansionFunction_clicked()
{
    //展开功能
    ui->frame_3->setVisible(true);
    ui->frame_4->setVisible(true);
    ui->frame_8->setVisible(true);

    ui->toolBtnDropFunction->setVisible(true);
    ui->toolBtnExpansionFunction->setVisible(false);
}


void MainWindow::on_toolBtnChangeHorScreen_clicked()
{
    qDebug()<<"切换到横屏";
    ui->toolBtnChangeVerScreen->setVisible(true);
    ui->toolBtnChangeHorScreen->setVisible(false);
    ui->listWidget->clear();

    PhoneItemWidget* widget = NULL;
    QListWidgetItem* item = NULL;
    //int iCount = map.size();
    //qDebug() << "map size() =" << iCount;
    //QMap<int, S_ItemWidgetData*>::const_iterator iter = map.constBegin();
    //for (; iter != map.constEnd(); iter++)
    QString strImage;
    for (int i = 0; i < 1; i++)
    {
        /*switch (i)
        {
        case LEVEL_NOMAL_LEVEL:
            strImage = ":/main/main/level_normal.png";
            break;
        case LEVEL_ENHANCEMENT_TYPE:
            strImage = ":/main/main/level_enhancenment.png";
            break;
        case LEVEL_PREMIER_TYPE:
            strImage = ":/main/main/level_Premier.png";
            break;
        default:
            break;
        }*/
        widget = new PhoneItemWidget(this);

        item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(ITEM_PHONE_HORIZONTAL_WIDTH, ITEM_PHONE_HORIZONTAL_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        item->setData(Qt::UserRole, i);
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}


void MainWindow::on_toolBtnChangeVerScreen_clicked()
{
    qDebug()<<"切换到竖屏";
    ui->toolBtnChangeVerScreen->setVisible(false);
    ui->toolBtnChangeHorScreen->setVisible(true);
    ui->listWidget->clear();
    //初始化手机列表,假数据
    PhoneItemWidget* widget = NULL;
    QListWidgetItem* item = NULL;
    //int iCount = map.size();
    //qDebug() << "map size() =" << iCount;
    //QMap<int, S_ItemWidgetData*>::const_iterator iter = map.constBegin();
    //for (; iter != map.constEnd(); iter++)
    QString strImage;
    for (int i = 0; i < 1; i++)
    {
        /*switch (i)
        {
        case LEVEL_NOMAL_LEVEL:
            strImage = ":/main/main/level_normal.png";
            break;
        case LEVEL_ENHANCEMENT_TYPE:
            strImage = ":/main/main/level_enhancenment.png";
            break;
        case LEVEL_PREMIER_TYPE:
            strImage = ":/main/main/level_Premier.png";
            break;
        default:
            break;
        }*/
        widget = new PhoneItemWidget(this);

        item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(ITEM_PHONE_VERTICAL_WIDTH, ITEM_PHONE_VERTICAL_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        item->setData(Qt::UserRole, i);
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}


void MainWindow::on_toolBtnPayZhifubao_clicked()
{
    ui->toolBtnPayZhifubao->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_select.png);padding-left:10px;}");
    ui->toolBtnPayWechat->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_normal.png);padding-left:10px;}");
}


void MainWindow::on_toolBtnPayWechat_clicked()
{
    ui->toolBtnPayWechat->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_select.png);padding-left:10px;}");
    ui->toolBtnPayZhifubao->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_normal.png);padding-left:10px;}");
}

