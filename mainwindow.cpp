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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_DeleteOnClose, true);

    //����TreeWidget�������
    ui->treeWidget->resize(200, 600);
    ui->treeWidget->move(100, 80);

    ui->treeWidget->setColumnCount(1);
    //ui->treeWidget->setHeaderLabel("�����б�");
    //ui->treeWidget->setHeaderLabels("�����б�");
    //���ر�ͷ
    ui->treeWidget->setHeaderHidden(true);

    //���ø�ѡ��
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
    qDebug() << "��ת����ҳ��" << "id=" << userInfo.id << "name=" << userInfo.strName << "account=" << userInfo.strAccount << "mobile=" << userInfo.strMobile << "MaxExpirationDate" << userInfo.strMaxExpirationDate << "token=" << userInfo.strToken;
    QueryAllGroup();
}

void MainWindow::on_btnClose_clicked()
{
    //�رմ��ڲ����˳���¼
    qDebug() << "ע��";
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_LOGOUT;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
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
    //����GET����
    QByteArray postData = "";
    QNetworkReply* reply = manager->post(request, postData);
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished,this, [=] {
            //��ȡ��Ӧ����
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
                            qDebug() << "��ת����ҳ��"<<"id="<<id<<"name="<<strName<<"account="<<strAccount<<"mobile="<<strMobile<<"MaxExpirationDate"<<strMaxExpirationDate<<"token="<<strToken;
                        }*/
                        qDebug()<<"ע���ɹ�";
                    }
                    else
                    {
                        QMessageBox::warning(this, tr("������ʾ"), strMessage);
                    }
                }
            }
        reply->deleteLater();
    });

    this->close();
}

void MainWindow::QueryAllGroup()//��ѯȫ������
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_QUERY_ALL_GROUP;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
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
    //����GET����
    QByteArray postData = "";
    QNetworkReply* reply = manager->post(request, postData);
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                            //�����б���ʾ
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

                            //����UI�ӿ���ʾ����
                            ShowGroupInfo();
                        }
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
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
        //�洢GroupId
        item->setData(0, Qt::UserRole, iter->iGroupId);
        ui->treeWidget->addTopLevelItem(item);
        
        //����ӽڵ�
        child = new QTreeWidgetItem(item);
        child->setText(0, iter->strGroupName + "�ӽڵ�");
        //���ڵ���ӵ����ڵ���
        item->addChild(child);

        child = new QTreeWidgetItem(item);
        child->setText(0, iter->strGroupName + "�ӽڵ�1");
        //���ڵ���뵽��һ��λ��
        //item->addChild(child);
        //ͬ��
        ui->treeWidget->insertTopLevelItem(0, child);
    }

    //չ������
    ui->treeWidget->expandAll();
}

void MainWindow::CreateGroup(QString strGroupName)//��������
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_GROUP;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
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
    //����GET����
    QNetworkReply* reply = manager->post(request, postData);
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                    //������Ӹ���
                    QueryAllGroup();
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

void MainWindow::UpdateGroup(int iGroupId, QString strNewName)//�޸ķ���
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_UPDATE_GROUP;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
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
    //����GET����
    QNetworkReply* reply = manager->post(request, postData);
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                    //true�����ɹ�
                    if (data)
                    {
                        //����ֱ���޸����Ʋ���Ҫ��������
                        QueryAllGroup();
                    }                    
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

void MainWindow::DeleteGroup(int iGroupId)//ɾ������
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_GROUP;
    strUrl += QString("/%1").arg(iGroupId);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
    qDebug() << "strUrl = " << strUrl;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);
    
    //����GET����
    QNetworkReply* reply = manager->post(request, "");
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                    //true�����ɹ�
                    if (data)
                    {
                        //����ֱ���޸����Ʋ���Ҫ��������
                        QueryAllGroup();
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

void MainWindow::on_btnCreateNewGroup_clicked()
{
    //�½�����
    /*CreateGroupWidget* createGroupWidget = new CreateGroupWidget();
    connect(createGroupWidget, &CreateGroupWidget::createGroupSignals, this,[this](QString strGroupName)
            {
        //��������
        qDebug()<<" �������� strGroupName="<<strGroupName;
        CreateGroup(strGroupName);
    });
    createGroupWidget->show();*/


    //�����޸ķ���ӿ�
    //UpdateGroup(2, "������");

    //����ɾ������ӿ�
    //DeleteGroup(0);

    //���Ի�ȡserverToken�ӿ�
    GetMyPhoneInstance();
}


void MainWindow::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    if(qApp->mouseButtons() == Qt::RightButton) // ֻ�������Ҽ�
    {
        QTreeWidget* tree = item->treeWidget(); // ��ȡ��ǰitem���ڵ�QTreeWidget

        // [option] �˴��������������ֻ���ָ����QTreeWidgetItem������Ҽ��˵����ܡ�

        QMenu* menu = new QMenu(tree);
        QAction* action1 = new QAction("ɾ������");
        QAction* action2 = new QAction("�༭��������");
        menu->addAction(action1);
        menu->addAction(action2);

        // Ϊ�Ҽ��˵��ϵ�QAction�����źŲۣ���Ӷ�Ӧ�Ĺ���
        connect(action1, &QAction::triggered, this, [this](bool bCheck)
                {
                    int iGroupId = 0;
                    QTreeWidgetItem* selectItem = ui->treeWidget->currentItem();
                    if(selectItem != NULL)
                    {
                        iGroupId = selectItem->data(0, Qt::UserRole).toInt();
                    }
                    DeleteGroup(iGroupId);
                });

        connect(action2, &QAction::triggered, this,[this](bool bCheck)
                {
                    UpdateGroupWidget* updateGroupWidget = new UpdateGroupWidget();
                    connect(updateGroupWidget, &UpdateGroupWidget::updateGroupSignals, this,[this](QString strGroupName)
                            {
                                int iGroupId = 0;
                                QTreeWidgetItem* selectItem = ui->treeWidget->currentItem();
                                if(selectItem != NULL)
                                {
                                    iGroupId = selectItem->data(0, Qt::UserRole).toInt();
                                }
                                //�޸ķ�������
                                qDebug()<<" �޸ķ������� strGroupName="<<strGroupName;
                                UpdateGroup(iGroupId,strGroupName);
                            });
                    updateGroupWidget->show();
                });

        // �Ҽ��˵����������λ����ʾ
        menu->exec(QCursor::pos());
    }
}


void MainWindow::on_btnRefresh_clicked()
{
    qDebug()<<"ˢ��";
    //���¼����б�
    //QueryAllGroup();

    //���Ի�ȡSeverToken�ӿ�
    GetMyPhoneInstance();
}


//��ȡserverToken
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
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //����GET����
    QNetworkReply* reply = manager->post(request, "");
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                    qDebug() << "�����ɹ�";
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
    });
}*/

// �����ӿ�-�ҵ�֧������
void MainWindow::HttpGetMyOrder(int iPage,int iPageSize)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_ORDER;
    strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //����GET����
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                            //��ȡ�ҵ��ֻ�ʵ�����ݣ���δ�洢
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
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

//�����ӿ�-��������
void MainWindow::HttpCreateOrder(int iChannel,int iMemberId,int iNum,QString strRelateId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_ORDER;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
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
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                            //�����б���ʾ
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

                            //����UI�ӿ���ʾ����
                            ShowGroupInfo();
                        }
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

//�رն���
void MainWindow::HttpCloseOrder(QString strOutTradeNo)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += "/api/order/closeOrder";// HTTP_CLOSE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(strOutTradeNo);
    qDebug() << "strUrl = " << strUrl;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    QNetworkReply* reply = manager->post(request, "");
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                        qDebug() << "�رն����ɹ�";
                    else
                        qDebug() << "�رն���ʧ��";
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

//ɾ��
void MainWindow::HttpDeleteOrder(int iOrderId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(iOrderId);
    qDebug() << "strUrl = " << strUrl;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    QNetworkReply* reply = manager->post(request, "");
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                        qDebug() << "ɾ�������ɹ�";
                    else
                        qDebug() << "ɾ������ʧ��";
                }
                else
                {
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}

//���
void MainWindow::HttpEmptyOrder()
{

}

//��ȡ�ҵ��ֻ�ʵ��
void MainWindow::GetMyPhoneInstance()
{
    int iGroupId = 0;
    int iLevel = 0;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_PHONE_INSTANCE;
    strUrl += QString("?groupId=0&level=0&page=1&pageSize=10");
    qDebug() << "strUrl = " << strUrl;
    //����������ʹ�����,����ָ�뺯���������ͷ���˲������finished�Ĳ�
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //�����������
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //����GET����
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //����������ɵ��ź�
    connect(reply, &QNetworkReply::finished, this, [=] {
        //��ȡ��Ӧ����
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
                            //��ȡ�ҵ��ֻ�ʵ�����ݣ���δ�洢
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
                    QMessageBox::warning(this, tr("������ʾ"), strMessage);
                }
            }
        }
        reply->deleteLater();
        });
}
void MainWindow::on_btnCreateOrder_clicked()
{
    //��������
    HttpCreateOrder(0,m_userInfo.id,1,"");
}


void MainWindow::on_btnMyOrder_clicked()
{
    //�ҵ�֧������
    HttpGetMyOrder(0,10);
}


void MainWindow::on_btnCloseOrder_clicked()
{
    QString strOutTradeNo;
    //��ȡ�رն�����

    HttpCloseOrder(strOutTradeNo);
}


void MainWindow::on_btnDeleteOrder_clicked()
{
    int iOrderId=0;
    //��ȡɾ������ID


    HttpDeleteOrder(iOrderId);
}


void MainWindow::on_btnPayOrder_clicked()
{
    //ȷ��֧��
    //����ҳ��ֵ
    //https://excashier.alipay.com/standard/auth.htm?payOrderId=fe951ef41b1b4a85af044f5f7cf9c4e1.00

}


void MainWindow::on_btnMax_clicked()
{

}

