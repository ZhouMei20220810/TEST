#ifndef CREATEGROUPWIDGET_H
#define CREATEGROUPWIDGET_H

#include <QWidget>

namespace Ui {
class CreateGroupWidget;
}

class CreateGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateGroupWidget(QWidget *parent = nullptr);
    ~CreateGroupWidget();
signals:
    void createGroupSignals(QString strGroupName);
private slots:
    void on_btnOk_clicked();

private:
    Ui::CreateGroupWidget *ui;
};

#endif // CREATEGROUPWIDGET_H
