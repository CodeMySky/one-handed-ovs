#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QList>
#include <QJsonArray>

#include "persistantservice.h"
#include "bridge.h"
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    QString getInfo(QString type, QStringList nameList);
    void addBridge(QString);
    void deleteBridge(QString);
    void addPort(QString, QString);
    void deletePort(QString, QString);
    void readNVO3(QString type);
public slots:
    void startOvs();
signals:
    void bridgeConfirmed(QString);
    void portConfirmed(int, QString);
    void interfaceConfirmed(int, int, QString);
    void execErrorConfirmed(QString);
    void nvo3DataConfirmed(QStringList);
    void clearAll();

public slots:
    void refreshOvs();
    void setInterface(QString interfaceName, QString type, QStringList options);

private:
    PersistantService *ps;
    QList<Bridge *> bridgeList;

private slots:
    void bridgeFound(QString bridgeName);
    void portFound(QString bridgeName, QString portName);
    void interfaceFound(QString bridgeName, QString portName, QString interfaceName);
    void interfaceTypeFound(QString bridgeName, QString portName, QString interfaceName, QString type);
    void interfaceAttrFound(QString bridgeName, QString portName, QString interfaceName, QString key, QString value);
    void execErrorFound(QString err);
};

#endif // CONTROLLER_H
