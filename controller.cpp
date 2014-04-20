#include "controller.h"
#include <QDebug>
Controller::Controller(QObject *parent) :
    QObject(parent)
{
    ps = new PersistantService;
    connect(ps,SIGNAL(bridgeFound(QString)),this,SLOT(bridgeFound(QString)));
    connect(ps,SIGNAL(portFound(QString,QString)),this,SLOT(portFound(QString,QString)));
    connect(ps, SIGNAL(interfaceFound(QString,QString,QString)),this,SLOT(interfaceFound(QString,QString,QString)));
    ps->startOvs();
    qDebug()<<"Controller init end";
}

void Controller::bridgeFound(QString bridgeName) {
    Bridge *br = new Bridge(bridgeName);
    bridgeList.append(br);
    emit bridgeConfirmed(bridgeName);
}

void Controller::portFound(QString bridgeName, QString portName) {
    for (int i=0;i<bridgeList.length();i++) {
        if (bridgeList[i]->getName() == bridgeName) {
            bridgeList[i]->addPort(portName);
            emit portConfirmed(i, portName);
            break;
        }
    }
}

void Controller::interfaceFound(QString bridgeName, QString portName, QString interfaceName) {
    for (int i=0;i<bridgeList.length();i++) {
        if (bridgeList[i]->getName() == bridgeName) {
            int portIndex = bridgeList[i]->indexOf(portName);
            if (portIndex > -1) {
                emit interfaceConfirmed(i, portIndex, interfaceName);
            }
            break;
        }
    }
}

void Controller::startOvs() {
    ps->startOvs();
}
