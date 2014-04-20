#ifndef PERSISTANTSERVICE_H
#define PERSISTANTSERVICE_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QTimer>

class PersistantService : public QObject
{
    Q_OBJECT
public:
    explicit PersistantService(QObject *parent = 0);

signals:
    void ovsStarted();
    void bridgeFound(QString);
    void portFound(QString, QString);
    void interfaceFound(QString, QString, QString);
    void interfaceTypeFound(QString, QString, QString, QString);
    void interfaceAttrFound(QString, QString, QString, QString, QString);
    void execErrorFound(QString);
    void needRefresh();

public:
    void startOvs();
    void listStatus();
    void addBridge(QString bridgeName);
    void deleteBridge(QString bridgeName);
    void addPort(QString bridgeName, QString portName);
    void deletePort(QString bridgeName, QString portName);

public slots:
private slots:
    //common
    void wakeWatchDog();
    void killProcess();
    void onNormalProcessEnd();
    //startOvs
    void onOvsServerStarted();
    void onOvsVswitchdStarted();
    //listStatus()
    void readStatus();
    void onListStatusEnd();
private:
    QProcess * process;
    QTimer * watchDog;
    QString currentBridge;
    QString currentPort;
    QString currentInterface;
};


#endif // PERSISTANTSERVICE_H
