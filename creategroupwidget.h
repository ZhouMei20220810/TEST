#ifndef CREATEGROUPWIDGET_H
#define CREATEGROUPWIDGET_H

#include "global.h"

namespace Ui {
class CreateGroupWidget;
}

class CreateGroupWidget : public QMoveDialog
{
    Q_OBJECT

public:
    explicit CreateGroupWidget(ENUM_CREATE_OR_UPDATA type,int id = 0,QString strLineEditText = "", QDialog* parent = nullptr);
    ~CreateGroupWidget();
signals:
    void createGroupSignals(ENUM_CREATE_OR_UPDATA type,QString strGroupName,int id);
private slots:
    void on_btnOk_clicked();

    void on_pushButton_2_clicked();

private:
    ENUM_CREATE_OR_UPDATA m_type;
    int m_id;
    Ui::CreateGroupWidget *ui;
};

#endif // CREATEGROUPWIDGET_H
