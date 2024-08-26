#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
class DataProvider : public QObject
{
    Q_OBJECT
public:
    //explicit DataProvider(QObject *parent = nullptr);
    explicit DataProvider(QObject* parent = nullptr) : QObject(parent)
    {
        // 初始化数据
        QString strTmpImg;
        for (int i = 1; i <= 4; ++i)
        {
            strTmpImg = QString("file://C:/Users/Administrator/AppData/Local/Temp/YiShunYun/background%2.png").arg(i % 2 + 1);
            addItem(QString("Window %1").arg(i), strTmpImg, true);
            qDebug() << "strTmpImg = " << strTmpImg;
        }
    }
    

    Q_INVOKABLE void addItem(const QString &name, const QString &backgroundImage, bool checked)
    {
        QVariantMap item;
        item["name"] = name;
        item["checked"] = checked;
        item["backgroundImage"] = QUrl::fromLocalFile(backgroundImage); // 注意这里使用 QUrl
        m_data.append(item);
        emit dataChanged();
    }

    QVariantList data() const { return m_data; }
signals:

    void dataChanged();

private:
    QVariantList m_data;
};

#endif // DATAPROVIDER_H
