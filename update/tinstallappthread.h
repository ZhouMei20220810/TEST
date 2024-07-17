#ifndef TINSTALLAPPTHREAD_H
#define TINSTALLAPPTHREAD_H

#include <QObject>
#include <QThread>

class TInstallAppThread : public QThread
{
    Q_OBJECT
public:
    TInstallAppThread(QObject* parent = nullptr);
signals:
    void showPrograssValueSignals(int value);
protected:
    void run() override;
};

#endif // TINSTALLAPPTHREAD_H
