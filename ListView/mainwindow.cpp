#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#include <QQuickWindow>
#include <QList>
#include <QVariant>
#include <QQmlListProperty>
#include <QVBoxLayout>
#include <QQmlContext>
//#include <QQmlEngine>
#include "qtsizemanager.h"
#include "qmlsizemanager.h"
#include "listitem.h"
#include "listmodel.h"
/*class SizeManager {
public:
    int currentSize() const { return m_currentSize; }
    void setCurrentSize(int size) { m_currentSize = size; }

private:
    int m_currentSize = 100; // 默认大小
};*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //通过qmlRegisterType注册的对象，在QML中一定要写一个QMLSizeManager{id:qmlSizeManager}
    //qmlRegisterType<QMLSizeManager>("QMLSizeManager", 1, 0, "QMLSizeManager");
    //通过一下方法可以不用在QML中声明，直接用QMLSizeManager即可
    QMLSizeManager::getInstance()->setCellWidth(237);
    QMLSizeManager::getInstance()->setCellHeight(426);
    qmlRegisterSingletonInstance("QMLSizeManager", 1, 0, "QMLSizeManager", QMLSizeManager::getInstance());    
    qmlRegisterSingletonInstance("MyListModel",1,0,"MyListModel", MyListModel::getInstance());
    //主窗口的子窗口设置QML布局
    m_quickWidget = new QQuickWidget(ui->widget);
    //QQmlApplicationEngine engine;
    //QQmlContext* content = engine.rootContext();    
    
    //m_quickWidget->setSource(QUrl(QStringLiteral("qrc:/listview.qml")));qrc:/Test.qml
    m_quickWidget->setSource(QUrl(QStringLiteral("qrc:/listview.qml"))); 
    //设置QQuickWidget 为父窗口的布局中心
    /*QVBoxLayout* layout = new QVBoxLayout(ui->widget);
    layout->addWidget(m_quickWidget);
    layout->setContentsMargins(0, 0, 0, 0);//移除边距
    m_quickWidget->show();*/

    // 注册 SizeManager 到 QML 上下文中
    /*QQmlEngine* engine = m_quickWidget->engine();
    QQmlContext* content = engine->rootContext();
    content->setContextProperty("WIDTH", 800);
    content->setContextProperty("HEIGHT", 600);
    content->setContextProperty("CELLWIDTH", 247);
    content->setContextProperty("CELLHEIGHT", 436);
    content->setContextProperty("ISVERTICALSCREEN",true); //横屏还是竖屏,控制宽高和图片旋转
    //content->setContextProperty("sizeManager", sizeManager);
    qmlRegisterType<QtSizeManager>("SizeManager",1,0,"QtSizeManager");

    // 连接 QComboBox 改变事件
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [&](int index) {
            QQmlEngine* engine = m_quickWidget->engine();
            QQmlContext* content = engine->rootContext();
            switch (index) {
            case 0: 
                content->setContextProperty("CELLWIDTH", 207 * 0.3);
                content->setContextProperty("CELLHEIGHT", 396 * 0.3);
                //sizeManager->setCurrentSize(0.3); 
                break;
            case 1: 
                content->setContextProperty("CELLWIDTH", 207*0.5);
                content->setContextProperty("CELLHEIGHT", 396*0.5);
                //sizeManager->setCurrentSize(0.5);
                break;
            case 2: 
                content->setContextProperty("CELLWIDTH", 207);
                content->setContextProperty("CELLHEIGHT", 396);
                //sizeManager->setCurrentSize(1); 
                break;
            default:
                break;
                // 可以继续添加更多的情况
            }
        });*/

    
    //ui->widget->setLayout(layout);

    /*QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    context->setContextProperty("SCREEN_WIDTH", 200);*/
    //context->setContextProperty("SCREEN_WIDTH", 200);


    /*qmlRegisterType<MainWindow>("MyApp", 1, 0, "MainWindow");

    // 加载 QML 文件
    engine.rootContext()->setContextProperty("mainWindow", this);
    //设置全局属性值
    //engine.rootContext()->setContextProperty("SCREEN_WIDTH", 600);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));*/
    /*qmlRegisterType<MainWindow>("MyApp", 1, 0, "MainWindow");

    // 加载 QML 文件
    engine.rootContext()->setContextProperty("mainWindow", this);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));*/

    /*m_model = new QStandardItemModel(ui->listView);

    ui->listView->setModel(m_model);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnChangeSize_clicked()
{
    MyListModel::getInstance()->removeAllItem();
    ListItem* item = NULL;
    QString strTemp = "C:/Users/Administrator/AppData/Local/Temp/YiShunYun";
    for (int i = 0; i < 5; i++)
    {
        item = new ListItem();
        item->setIndex(i);
        item->setPhoneName(QString("text%1").arg(i));
        item->setImagePath(QString("file:///%1/%2.png").arg(strTemp).arg(i));
        MyListModel::getInstance()->addItem(item);
    }
    //m_quickWidget->updateGeometry();
    //load engine 加载完成之后
    //QML信号 C++槽函数，在C++完成绑定，通过objectName访问
    /*QQmlApplicationEngine qmlEngine;
    QList<QObject*> objList = qmlEngine.rootObjects();
    qDebug() << objList.first()->objectName();*/

    // 创建 QQmlApplicationEngine 实例
    /*QQmlApplicationEngine engine;

    // 注册 C++ 类到 QML
    //qmlRegisterType<ListModel>("ListModel", 1, 0, "ListModel");
    //qmlRegisterType<ListItem>("ListItem", 1, 0, "ListItem");
    qmlRegisterType<QMLSizeManager>("QMLSizeManager", 1, 0, "QMLSizeManager");

    // 创建 ListModel 实例
    //ListModel* listModel = new ListModel(&engine);

    // 将 ListModel 注册到 QML 上下文中
    //engine.rootContext()->setContextProperty("listModel", listModel);

    // 加载 QML 文件
    engine.load(QUrl(QStringLiteral("qrc:/listview.qml")));

    // 检查是否有根对象
    if (engine.rootObjects().isEmpty()) {
        qDebug() << "Failed to load QML file.";
        return;
    }

    // 获取第一个根对象
    QObject* rootObject = engine.rootObjects().first();
    if (NULL == rootObject)
        return;
    qDebug() << rootObject->objectName();

    QObject* listview = rootObject->findChild<QObject*>("listView");
    if (listview == NULL)
    {
        return;        
    }
    qDebug() << listview << listview->objectName();

    //button不是listview直接子控件因此找不到
    QObject* button = listview->findChild<QObject*>("windowItem");
    if (button == NULL)
    {
        return;
    }
    qDebug() << button << button->objectName();
    QObject::connect(button,SIGNAL(qmlSendSignals(int,QString,QString)),QMLSizeManager::getInstance(), SLOT(receiveSignalFromQMLFile(int,QString,QString)));
    */
    // 在这里可以继续做其他操作，例如连接信号和槽等
    // 
    //QMLSizeManager::getInstance()->setCellWidth(500);
    /*QQmlApplicationEngine engine;
    QQmlContext* content = engine.rootContext();
    content->setContextProperty("CELL_WIDTH", 600);
    content->setContextProperty("CELL_HEIGHT", 500);
    //engine.load(QUrl(QStringLiteral("qrc:/listview.qml")));
    m_quickWidget->setSource(QUrl(QStringLiteral("qrc:/listview.qml")));
    //设置QQuickWidget 为父窗口的布局中心
    QVBoxLayout* layout = new QVBoxLayout(ui->widget);
    layout->addWidget(m_quickWidget);
    layout->setContentsMargins(0, 0, 0, 0);//移除边距
    m_quickWidget->show();*/
    /*if (engine.rootObjects().isEmpty()) {
        qFatal("Failed to load the QML file.");
    }

    // 获取 QML 对象
    QObject* rootObjectQObject = engine.rootObjects().first();
    QQuickItem* rootObject = dynamic_cast<QQuickItem*>(rootObjectQObject);
    if (!rootObject) {
        qWarning("Failed to cast 'rootObject' to QQuickItem.");
        return;
    }

    // 使用 findChild 获取 ListView
    QObject* listViewQObject = rootObject->findChild<QObject*>("listView");
    QQuickItem* listView = dynamic_cast<QQuickItem*>(listViewQObject);
    if (!listView) {
        qWarning("Failed to cast 'listView' to QQuickItem.");
        return;
    }

    // 获取 ListView 的模型
    QVariant modelVariant = listView->property("model");
    if (!modelVariant.isValid()) {
        qWarning("The model property of the ListView is not valid.");
        return;
    }

    // 尝试将 QVariant 转换成 QAbstractListModel
    QAbstractListModel* model = qvariant_cast<QAbstractListModel*>(modelVariant);
    if (!model) {
        qWarning("Failed to cast the model property to QAbstractListModel.");
        return;
    }

    // 获取模型的行数
    int count = model->rowCount();
    if (count > 0) {
        QVariant data = model->data(model->index(0, 0));
        if (data.canConvert<QObject*>()) {
            QObject* item = data.value<QObject*>();
            QQuickItem* itemAsQQuickItem = dynamic_cast<QQuickItem*>(item);
            if (itemAsQQuickItem) {
                itemAsQQuickItem->setProperty("width", 300); // 宽度设为 300
                itemAsQQuickItem->setProperty("height", 150); // 高度设为 150
            }
            else {
                qWarning("Failed to cast the first item to QQuickItem.");
            }
        }
        else {
            qWarning("The first item in the model is not a QObject*.");
        }
    }
    else {
        qWarning("No items found in the model.");
    }
    return;*/


    /*if (engine.rootObjects().isEmpty()) {
        qFatal("Failed to load the QML file.");
    }

    // 获取 QML 对象
    QObject* rootObjectQObject = engine.rootObjects().first();
    //QWidget* rootObject = dynamic_cast<QWidget*>(rootObjectQObject);
    //if (!rootObject) {
    //    qWarning("Failed to cast 'rootObject' to QQuickItem.");
    //    return;
    //}
    //ui->listView->setModel()

    // 使用 findChild 获取 ListView
    QObject* listViewQObject = rootObjectQObject->findChild<QObject*>("listView");
    QQuickItem* listView = dynamic_cast<QQuickItem*>(listViewQObject);
    if (!listView) {
        qWarning("Failed to cast 'listView' to QQuickItem.");
        return;
    }

    // 使用 findChildren 获取所有的 windowItem 对象
    QList<QObject*> windowItemObjects = listView->findChildren<QObject*>("windowItem");
    for (QObject* windowItemQObject : windowItemObjects) {
        QQuickItem* windowItem = dynamic_cast<QQuickItem*>(windowItemQObject);
        if (!windowItem) {
            qWarning("Failed to cast 'windowItem' to QQuickItem.");
            continue;
        }

        // 修改 Rectangle 的宽度和高度
        windowItem->setProperty("width", 300); // 宽度设为 300
        windowItem->setProperty("height", 150); // 高度设为 150
    }*/
}


void MainWindow::on_btnAdd_clicked()
{
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return;
    engine.rootContext()->setContextProperty("mainWindow", this);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        qFatal("Failed to load the QML file.");
    }

    // 获取 QML 对象
    QObject* rootObjectQObject = engine.rootObjects().first();

    QQuickItem* rootObject = dynamic_cast<QQuickItem*>(rootObjectQObject);
    if (!rootObject) {
        qWarning("Failed to cast 'rootObject' to QQuickItem.");
        return;
    }

    // 使用 findChild 获取 ListView
    QObject* listViewQObject = rootObject->findChild<QObject*>("listView");
    QQuickItem* listView = dynamic_cast<QQuickItem*>(listViewQObject);
    if (!listView) {
        qWarning("Failed to cast 'listView' to QQuickItem.");
        return;
    }

    //addItemsToModel(listView);
}

void MainWindow::fillListModel(QQmlListProperty<QObject>* model)
{
    // 清空现有的模型
    /*model->clear(model);//clear();

    // 填充模型
    for (int i = 0; i < 1000; ++i) 
    {
        QQmlListProperty<QObject> listElement;
        QQmlListPropertyAppend append(&listElement);
        append(model, new QQmlListPropertyPrivate());
        QQmlListPropertyAppend appendElement(&listElement);
        appendElement(model, new QQmlListPropertyPrivate(QString("Window %1").arg(i + 1)));
    }*/
}

// 动态添加数据到 ListModel
//void MainWindow::addItemsToModel()
//{
    /*QVariant modelVariant = listView->property("model");
    QQmlListProperty<QObject>* model = qvariant_cast<QQmlListProperty<QObject>*>(modelVariant);

    for (int i = 0; i < 1000; ++i) {
        QVariantMap itemData;
        itemData.insert("name", QString("Window %1").arg(i + 1));
        model->append(itemData);
    }*/
//}

// 动态添加数据到 ListModel
//void MainWindow::addItemsToModel(QQuickItem* listView)
//{
    // 创建一个新的 ListModel
    /*QList<QVariantMap> items;
    for (int i = 0; i < 1000; ++i) {
        QVariantMap itemData;
        itemData.insert("name", QString("Window %1").arg(i + 1));
        items.append(itemData);
    }

    // 将模型设置为 ListView 的模型
    QStandardItemModel model(items);
    listView->setProperty("model", model);*/
//}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    //int i = ui->comboBox->itemData(index).toInt();
    int i = ui->comboBox->itemText(index).toInt();
    //qDebug() << "click i=" << i << "old width=" << GlobalData::iPhoneItemWidth << "old height=" << GlobalData::iPhoneItemHeight;
    int iWidth = 207* (i / 100.0);
    int iHeight = 396 * (i / 100.0);


    QMLSizeManager::getInstance()->setCellWidth(iWidth);
    QMLSizeManager::getInstance()->setCellHeight(iHeight);
    qDebug() << "click i=" << i << "old width=" << iWidth << "old height=" << iWidth;

}

