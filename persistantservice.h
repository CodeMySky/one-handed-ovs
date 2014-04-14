#ifndef PERSISTANTSERVICE_H
#define PERSISTANTSERVICE_H

#include <QObject>
#include <QProcess>
#include <QStringList>
#include <QMutex>
class PersistantService : public QObject
{
    Q_OBJECT
public:
    explicit PersistantService(QObject *parent = 0);

signals:
    void isOVSRunning();
    void ovsStarted();
    void bridgeFound(QString);
    void portFound(QString,QString);
public:
    void run();
    void startOvs();
    void listBridges();
    void listPorts(QString bridgeName, bool hasWaited = false);
public slots:
private slots:
    void readOutput();
    void onStarted();
    void onFinished();
    //startOvs
    void onOvsServerStarted();
    void onOvsVswitchdStarted();
    //listBridges()
    void readBridge();
    void onListBridgeEnd();
    //listPorts()
    void readPort();
    void onListPortEnd();
private:
    QProcess * process;
    QStringList tempBridgeList;
};


#endif // PERSISTANTSERVICE_H
