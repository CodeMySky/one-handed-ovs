#include "controller.h"
#include <QDebug>
Controller::Controller(QObject *parent) :
    QObject(parent)
{
    ps = new PersistantService;
    connect(ps,SIGNAL(bridgeFound(QString)),this,SLOT(bridgeFound(QString)));
    connect(ps,SIGNAL(portFound(QString,QString)),this,SLOT(portFound(QString,QString)));
    ps->startOvs();
    qDebug()<<"Controller init end";
}

void Controller::bridgeFound(QString bridgeName) {
    emit bridgeConfirmed(bridgeName);
    bridgeList.append(bridgeName);
    ps->listPorts(bridgeName);
}

void Controller::portFound(QString bridgeName, QString portName) {
    emit portConfirmed(bridgeList.indexOf(bridgeName),portName);
}

void Controller::startOvs() {
    ps->startOvs();
}
