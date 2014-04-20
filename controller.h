#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QList>
#include <QStandardItemModel>
#include "persistantservice.h"
#include "bridge.h"
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);
    void startOvs();
    QString getInfo(QString type, QStringList nameList);
    void addBridge(QString);
    void deleteBridge(QString);

signals:
    void bridgeConfirmed(QString);
    void portConfirmed(int, QString);
    void interfaceConfirmed(int, int, QString);
    void execErrorConfirmed(QString);
    void clearAll();

public slots:
    void refreshOvs();

private:
    PersistantService *ps;
    QList<Bridge *> bridgeList;

private slots:
    void bridgeFound(QString bridgeName);
    void portFound(QString bridgeName, QString portName);
    void interfaceFound(QString bridgeName, QString portName, QString interfaceName);
    void interfaceTypeFound(QString bridgeName, QString portName, QString interfaceName, QString type);
    void execErrorFound(QString err);
};

#endif // CONTROLLER_H
