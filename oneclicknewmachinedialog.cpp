#include "oneclicknewmachinedialog.h"
#include "ui_oneclicknewmachinedialog.h"
#include <QGraphicsDropShadowEffect>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"

OneClickNewMachineDialog::OneClickNewMachineDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OneClickNewMachineDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("一键新机");

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);
    HttpGetModelList("",1, 1000);
}

OneClickNewMachineDialog::~OneClickNewMachineDialog()
{
    delete ui;
}

void OneClickNewMachineDialog::on_btnClose_clicked()
{
    this->close();
}


void OneClickNewMachineDialog::on_btnCancel_clicked()
{
    //取消
    this->close();
}


void OneClickNewMachineDialog::on_btnOk_clicked()
{
    //确定修改

}

//获取品牌列表
void OneClickNewMachineDialog::HttpGetModelList(QString strBrand,int iPage, int iPageSize)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MODEL_LIST;
    if(strBrand.isEmpty())
        strUrl += QString::asprintf("?page=%d&pageSize=%d", iPage, iPageSize);
    else
        strUrl += QString("?page=%1&pageSize=%2&brand=%3").arg(iPage).arg(iPageSize).arg(strBrand);
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);
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
                    QJsonObject data = obj["data"].toObject();
                    QJsonArray records = data["records"].toArray();
                    if (records.size() > 0)
                    {
                        int iRecordsSize = records.size();
                        QJsonObject recordObj;
                        //获取我的手机实例数据，暂未存储
                        S_BRAND_INFO info;
                        QMap<QString, S_BRAND_INFO> map;
                        for (int i = 0; i < iRecordsSize; i++)
                        {
                            recordObj = records[i].toObject();
                            info.id = recordObj["id"].toInt();
                            info.strModel = recordObj["model"].toString();
                            info.strBrand = recordObj["brand"].toString();
                            info.strManufacturer = recordObj["manufacturer"].toString();
                            if (strBrand.isEmpty())
                                map.insert(info.strBrand, info);
                            else
                                map.insert(info.strModel, info);
                        }
                        if (strBrand.isEmpty())
                            LoadBrandList(map);
                        else
                            LoadModelList(map);
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

void OneClickNewMachineDialog::LoadBrandList(QMap<QString, S_BRAND_INFO> map)
{
    QMap<QString, S_BRAND_INFO>::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        ui->comboBoxBrand->addItem(iter->strBrand);
    }    
}

void OneClickNewMachineDialog::LoadModelList(QMap<QString, S_BRAND_INFO> map)
{
    QMap<QString, S_BRAND_INFO>::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        ui->comboBoxModel->addItem(iter->strModel);
    }
}
void OneClickNewMachineDialog::on_comboBoxBrand_currentIndexChanged(int index)
{
    //当前下表改变
    QString strBrand = ui->comboBoxBrand->currentText();
    qDebug() << "select brand " << strBrand;
    ui->comboBoxModel->clear();
    HttpGetModelList(strBrand, 1, 1000);
}

