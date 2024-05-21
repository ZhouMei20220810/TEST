#ifndef CREATEGROUPWIDGET_H
#define CREATEGROUPWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class CreateGroupWidget;
}

class CreateGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroupWidget(ENUM_CREATE_OR_UPDATA type,QWidget *parent = nullptr);
    ~CreateGroupWidget();
signals:
    void createGroupSignals(ENUM_CREATE_OR_UPDATA type,QString strGroupName);
private slots:
    void on_btnOk_clicked();

private:
    ENUM_CREATE_OR_UPDATA m_type;
    Ui::CreateGroupWidget *ui;
};

#endif // CREATEGROUPWIDGET_H
