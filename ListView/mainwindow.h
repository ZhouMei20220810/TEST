#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlApplicationEngine>
#include <QStandardItemModel>
#include <QQuickWidget>

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
    void on_btnChangeSize_clicked();

    void on_btnAdd_clicked();
private:
    //填充列表模型
    void fillListModel(QQmlListProperty<QObject>* model);
    //动态添加数据
    //void addItemsToModel();
    // 动态添加数据到 ListModel
    //void addItemsToModel(QQuickItem* listView);
private:
    Ui::MainWindow *ui;
    QQmlApplicationEngine engine;

    QStandardItemModel* m_model;
    QQuickWidget* m_quickWidget;
};
#endif // MAINWINDOW_H
