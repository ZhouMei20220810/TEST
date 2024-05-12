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
#include <QMessageBox>
#include <QTreeWidget>
#include <QMenu>
#include "creategroupwidget.h"
#include "updategroupwidget.h"
#include "levelitemwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose,true);

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


    ui->listWidgetRenew->setHidden(true);
}


//激活码
void MainWindow::InitActiveCodeTab()
{

}

//购买
void MainWindow::InitBuyTab()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidgetLevel->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetLevel->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetLevel->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetLevel->setMovement(QListWidget::Static);

    //初始化列表云手机购买列表+云手机续费 等级列表
    LevelItemWidget* widget = NULL;
    QListWidgetItem* item = NULL;
    //int iCount = map.size();
    //qDebug() << "map size() =" << iCount;
    //QMap<int, S_ItemWidgetData*>::const_iterator iter = map.constBegin();
    //for (; iter != map.constEnd(); iter++)
    QString strImage;
    for (int i = 0; i < 3; i++)
    {
        switch(i)
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
        }
        widget = new LevelItemWidget(strImage,this);//(*iter.value(), this);

        item = new QListWidgetItem(ui->listWidgetLevel);
        item->setSizeHint(QSize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        item->setData(Qt::UserRole, i);
        ui->listWidgetLevel->addItem(item);
        ui->listWidgetLevel->setItemWidget(item, widget);
        //qDebug() << "listwidget load itemwidget record[ sceneId=" << iter.key() << "]";
    }
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
                }
            }
        }
        reply->deleteLater();
    });
}

//订单接口-创建订单
void MainWindow::HttpCreateOrder(int iChannel,int iMemberId,int iNum,QString strRelateId)
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
    obj.insert("relateId", strRelateId);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

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
                        if (0 == iGroupSize)
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
                            for (int i = 0; i < iGroupSize; i++)
                            {
                                dataObj = dataArray[i].toObject();
                                groupInfo.iGroupId = dataObj["id"].toInt();
                                groupInfo.iGroupNum = dataObj["num"].toInt();
                                groupInfo.strGroupName = dataObj["name"].toString();
                                qDebug() << "iGroupId=" << groupInfo.iGroupId << "strGroupName=" << groupInfo.strGroupName << "iGroupCount=" << groupInfo.iGroupNum;

                                m_mapGroupInfo.insert(i, groupInfo);
                            }

                            //调用UI接口显示数据
                            ShowGroupInfo();
                        }
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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

    ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->stackedWidget->setCurrentIndex(0);

}

//激活码
void MainWindow::on_toolBtnActiveCode_clicked()
{
    if(1 == ui->stackedWidget->currentIndex())
    {
        return;
    }
    ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->stackedWidget->setCurrentIndex(1);
}

//购买
void MainWindow::on_toolBtnBuy_clicked()
{
    if(2 == ui->stackedWidget->currentIndex())
    {
        return;
    }
    ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

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
                    QMessageBox::warning(this, tr("错误提示"), strMessage);
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
    ui->toolBtnRenewPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnBuyPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->listWidgetRenewList->setHidden(true);
}


void MainWindow::on_toolBtnRenewPhone_clicked()
{
    ui->toolBtnBuyPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");
    ui->toolBtnRenewPhone->setStyleSheet("QToolButton{border:none;color: rgb(204, 204, 204);background-color: rgb(41, 105, 205);border-radius:3px;}QToolButton:hover {color: rgb(255, 255, 255);background-color: rgb(41, 105, 205);}");

    ui->listWidgetRenewList->setHidden(false);
}


void MainWindow::on_btnBeginPay_clicked()
{
    //确定支付
//获取界面选中的值

}

void MainWindow::on_listWidgetLevel_itemClicked(QListWidgetItem *item)
{
    if (NULL == item)
        return;
    //选项更改
    //LevelItemWidget* levelItemWidget = static_cast<LevelItemWidget*>(ui->listWidgetLevel->itemWidget(item));
    //levelItemWidget->setLabelCheckStatus(true);

    LevelItemWidget* levelItemWidget = NULL;
    QListWidgetItem* levelItem=NULL;
    int iCount = ui->listWidgetLevel->count();
    for(int iRow=0; iRow < iCount;iRow++)
    {
        levelItem = ui->listWidgetLevel->item(iRow);
        levelItemWidget = static_cast<LevelItemWidget*>(ui->listWidgetLevel->itemWidget(levelItem));
        if (levelItem == item)
        {
            levelItemWidget->setLabelCheckStatus(true);
            //加载套餐列表
            //ui->listWidgetVIP

            //续费的列表
            //ui->listWidgetRenewList
        }
        else
            levelItemWidget->setLabelCheckStatus(false);
        //levelItem = static_cast<LevelItemWidget*>(ui->listWidgetLevel->item(iRow)->);
    }
}

