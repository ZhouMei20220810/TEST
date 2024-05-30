#ifndef TOOLOBJECT_H
#define TOOLOBJECT_H

#include <QObject>
#include "global.h"
class ToolObject : public QObject
{
    Q_OBJECT
public:
    explicit ToolObject(QObject *parent = nullptr);

    //手机实例相关接口
    //获取实例截图
    void HttpPostInstanceScreenshot(QStringList strList);
    //刷新实例截图
    void HttpPostInstanceScreenshotRefresh(QStringList strList);
    //实例重启
    void HttpPostInstanceReboot(QStringList strList);
    //实例重置
    void HttpPostInstanceReset(QStringList strList);
signals:
    void startTimerShowScreenshotSignals();
    void getScreenshortSignals(QMap<QString, S_TASK_INFO> mapScreenshotTask);
private:
    QMap<QString, S_TASK_INFO> m_mapScreenshotTask;
};

#endif // TOOLOBJECT_H
