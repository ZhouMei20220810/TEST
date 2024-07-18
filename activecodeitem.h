#ifndef ACTIVECODEITEM_H
#define ACTIVECODEITEM_H

#include <QWidget>

namespace Ui {
class ActiveCodeItem;
}

class ActiveCodeItem : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveCodeItem(QString strActiveCode,QString strStatus,QWidget *parent = nullptr);
    ~ActiveCodeItem();

    QString getActiveCode();
public slots:
    void do_activeCodeStatusSignals(QMap<QString, bool> mapActiveCodeStatus);
private:
    Ui::ActiveCodeItem *ui;
    QString m_strActiveCode;
};

#endif // ACTIVECODEITEM_H
