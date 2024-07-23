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
    void hideWindowSignals();
private:
    int installMsiSilently(const QString& msiFilePath, const QString& strExeFolder,const QString& strProductCode, int iPrograssValue);
protected:
    void run() override;
};

#endif // TINSTALLAPPTHREAD_H
