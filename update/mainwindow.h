#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tinstallappthread.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_toolBtnCancel_clicked();

    void on_btnClose_clicked();

    void do_showPrograssValueSignals(int value);
private:
    Ui::MainWindow *ui;
    TInstallAppThread* m_thread;
};
#endif // MAINWINDOW_H
