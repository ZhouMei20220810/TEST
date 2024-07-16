#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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

    void on_toolBtnUpdate_clicked();

    void on_btnClose_clicked();
private:
    void InstallApp();
private:
    Ui::MainWindow *ui;
    QString strExe;
    QTimer* m_Timer;
};
#endif // MAINWINDOW_H
