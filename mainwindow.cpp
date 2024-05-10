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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置TreeWidget相关属性
    ui->treeWidget->resize(200, 600);
    ui->treeWidget->move(100, 80);

    ui->treeWidget->setColumnCount(1);
    //ui->treeWidget->setHeaderLabel("分组列表");
    //ui->treeWidget->setHeaderLabels("分组列表");
    //隐藏表头
    ui->treeWidget->setHeaderHidden(true);

    //设置复选框
    ui->treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    //ui->treeWidget->setCheckBoxes(true);

    ui->treeWidget->expandAll();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUserInfo(S_USER_LOGIN_INFO userInfo)
{
    m_userInfo = userInfo;
    qDebug() << "跳转到主页面" << "id=" << userInfo.id << "name=" << userInfo.strName << "account=" << userInfo.strAccount << "mobile=" << userInfo.strMobile << "MaxExpirationDate" << userInfo.strMaxExpirationDate << "token=" << userInfo.strToken;
    QueryAllGroup();
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
                    if(200 == iCode)
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
                    }
                    else
                    {
                        QMessageBox::warning(this, tr("错误提示"), strMessage);
                    }
                }
            }
        reply->deleteLater();
    });

    this->close();
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
                if (200 == iCode)
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
        //item->addChild(child);
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
                if (200 == iCode)
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
                if (200 == iCode)
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
                if (200 == iCode)
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

void MainWindow::on_btnCreateNewGroup_clicked()
{
    //新建分组
    QString strNewGroup = "新建分组";
    //创建分组
    CreateGroup(strNewGroup);

    //调试修改分组接口
    //UpdateGroup(2, "新名称");

    //调试删除分组接口
    //DeleteGroup(0);
}


void MainWindow::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    if(qApp->mouseButtons() == Qt::RightButton) // 只针对鼠标右键
    {
        QTreeWidget* tree = item->treeWidget(); // 获取当前item所在的QTreeWidget

        // [option] 此处可以添加条件来只针对指定的QTreeWidgetItem来添加右键菜单功能。

        QMenu* menu = new QMenu(tree);
        QAction* action1 = new QAction("删除分组");
        QAction* action2 = new QAction("编辑分组名称");
        menu->addAction(action1);
        menu->addAction(action2);

        // 为右键菜单上的QAction创建信号槽，添加对应的功能
        connect(action1, &QAction::triggered, this, [this](bool bCheck)
                {
                    //QMessageBox::warning(this, "Action", "Open folder");
                    int iGroupId = 0;
                    //获取选中的id
                    DeleteGroup(iGroupId);
                });

        connect(action2, &QAction::triggered, this,[this](bool bCheck)
                {
                    int iGroupId = 0;
                    QString strNewName = "新";
                    UpdateGroup(iGroupId,strNewName);
                    //QMessageBox::warning(this, "Action", "edit file");
                });

        // 右键菜单在鼠标点击的位置显示
        menu->exec(QCursor::pos());
    }
}

