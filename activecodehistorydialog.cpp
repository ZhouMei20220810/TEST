#include "activecodehistorydialog.h"
#include "ui_activecodehistorydialog.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetipsdialog.h"
#include "messagetips.h"
#include "activecodehistoryitem.h"

ActiveCodeHistoryDialog::ActiveCodeHistoryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ActiveCodeHistoryDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("激活码记录");

    m_mapActiveInfo.clear();
    HttpGetAcitveCodeHistory(1,1000);
}

ActiveCodeHistoryDialog::~ActiveCodeHistoryDialog()
{
    delete ui;
}

void ActiveCodeHistoryDialog::on_btnClose_clicked()
{
    this->close();
}

void ActiveCodeHistoryDialog::HttpGetAcitveCodeHistory(int iPage, int iPageSize)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_ACTIVECODE_LIST;
    //strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("page", iPage);
    obj.insert("pageSize", iPageSize);
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
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_ACTIVE_CODE_DETAIL_INFO detailInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                detailInfo.strAccount = recordObj["account"].toString();
                                detailInfo.strBatchCode = recordObj["batchCode"].toString();
                                detailInfo.strActiveCode = recordObj["code"].toString();
                                detailInfo.strCreateTime = recordObj["createTime"].toString();
                                detailInfo.strExpireTime = recordObj["expireTime"].toString();
                                detailInfo.id = recordObj["id"].toInt();
                                detailInfo.strInstanceCreateTime = recordObj["instanceCreateTime"].toString();
                                detailInfo.strInstanceExpireTime = recordObj["instanceExpireTime"].toString();
                                detailInfo.bIsBind = recordObj["isBind"].toBool();
                                detailInfo.bIsUse = recordObj["isUse"].toBool();
                                detailInfo.iLevel = recordObj["level"].toInt();
                                detailInfo.strMobile = recordObj["mobile"].toString();
                                detailInfo.strInstanceNo = recordObj["no"].toString();
                                detailInfo.iRelateId = recordObj["relateId"].toInt();
                                detailInfo.iUseDay = recordObj["useDay"].toInt();
                                detailInfo.iType = recordObj["type"].toInt();
                                m_mapActiveInfo.insert(i, detailInfo);
                            }

                            //显示数据
                            ShowActiveCodeInfoList();
                        }
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//显示数据
void ActiveCodeHistoryDialog::ShowActiveCodeInfoList()
{
    if (m_mapActiveInfo.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("没有激活记录!");
        tips->show();
        return;
    }

    QListWidgetItem* item = NULL;
    ActiveCodeHistoryItem* itemWidget;
    QMap<int, S_ACTIVE_CODE_DETAIL_INFO>::iterator iter = m_mapActiveInfo.begin();
    for (; iter != m_mapActiveInfo.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetActiveCodeHistory);
        ui->listWidgetActiveCodeHistory->addItem(item);

        itemWidget = new ActiveCodeHistoryItem(*iter,this);
        item->setSizeHint(QSize(ui->listWidgetActiveCodeHistory->width()-50, itemWidget->height()));
        ui->listWidgetActiveCodeHistory->setItemWidget(item, itemWidget);
    }
}
