#ifndef UPDATEGROUPWIDGET_H
#define UPDATEGROUPWIDGET_H

#include <QWidget>

namespace Ui {
class UpdateGroupWidget;
}

class UpdateGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UpdateGroupWidget(QWidget *parent = nullptr);
    ~UpdateGroupWidget();
signals:
    void updateGroupSignals(QString strGroupName);
private slots:
    void on_btnOk_clicked();

private:
    Ui::UpdateGroupWidget *ui;
};

#endif // UPDATEGROUPWIDGET_H
