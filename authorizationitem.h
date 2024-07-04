#ifndef AUTHORIZATIONITEM_H
#define AUTHORIZATIONITEM_H

#include <QWidget>

namespace Ui {
class authorizationItem;
}

class authorizationItem : public QWidget
{
    Q_OBJECT

public:
    explicit authorizationItem(QWidget *parent = nullptr);
    ~authorizationItem();

private slots:
    void on_toolBtnOper_clicked();

private:
    Ui::authorizationItem *ui;
};

#endif // AUTHORIZATIONITEM_H
