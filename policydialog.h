#ifndef POLICYDIALOG_H
#define POLICYDIALOG_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class PolicyDialog;
}

class PolicyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolicyDialog(QString strTitle,QString strUrl,QWidget *parent = nullptr);
    ~PolicyDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::PolicyDialog *ui;

    QPoint m_dragPosition;
    // QWidget interface
/*protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;*/
};

#endif // POLICYDIALOG_H
