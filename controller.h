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
signals:
    void bridgeConfirmed(QString);
    void portConfirmed(int, QString);
public slots:
    //void showBridge();

private:
    PersistantService *ps;
    QList<QString> bridgeList;
    QStandardItemModel *model;
private slots:
    void bridgeFound(QString bridgeName);
    void portFound(QString bridgeName, QString portName);
};

#endif // CONTROLLER_H
