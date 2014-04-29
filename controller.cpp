#include "controller.h"
#include <QDebug>
Controller::Controller(QObject *parent) :
    QObject(parent)
{
    ps = new PersistantService;
    connect(ps,SIGNAL(bridgeFound(QString)),this,SLOT(bridgeFound(QString)));
    connect(ps,SIGNAL(portFound(QString,QString)),this,SLOT(portFound(QString,QString)));
    connect(ps, SIGNAL(interfaceFound(QString,QString,QString)),this,SLOT(interfaceFound(QString,QString,QString)));
    connect(ps, SIGNAL(interfaceTypeFound(QString,QString,QString,QString)),
            this, SLOT(interfaceTypeFound(QString,QString,QString,QString)));
    connect(ps, SIGNAL(interfaceAttrFound(QString,QString,QString,QString,QString)),
            this, SLOT(interfaceAttrFound(QString,QString,QString,QString,QString)));
    connect(ps, SIGNAL(execErrorFound(QString)), this, SLOT(execErrorFound(QString)));
    connect(ps,SIGNAL(needRefresh()),this,SLOT(refreshOvs()));
    connect(ps, SIGNAL(ovsStarted()),this,SLOT(refreshOvs()));
    ps->startOvs();
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
                bridgeList[i]->getPort(portName)->addInterface(interfaceName);
                emit interfaceConfirmed(i, portIndex, interfaceName);
            }
            break;
        }
    }
}

void Controller::interfaceTypeFound(QString bridgeName, QString portName, QString interfaceName, QString type) {
    for (int i=0;i<bridgeList.length();i++) {
        if (bridgeList[i]->getName() == bridgeName) {
            bridgeList[i]->getPort(portName)->getInterface(interfaceName)->setType(type);
        }
    }
}

void Controller::interfaceAttrFound(QString bridgeName, QString portName, QString interfaceName, QString key, QString value){
    for (int i=0;i<bridgeList.length();i++) {
        if (bridgeList[i]->getName() == bridgeName) {
            bridgeList[i]->getPort(portName)->getInterface(interfaceName)->setOptions(key, value);
        }
    }
}

QString Controller::getInfo(QString type, QStringList nameList) {
    if (type == "Bridge") {
        for (int i=0;i<bridgeList.length();i++) {
            if (bridgeList[i]->getName() == nameList[0]) {
                QStringList info = bridgeList[i]->info();
                return info.join(" ");
            }
        }
    } else if (type == "Port") {
        if (nameList.length() == 2) {
            for (int i=0;i<bridgeList.length();i++) {
                if (bridgeList[i]->getName() == nameList[0]) {
                    QStringList info = bridgeList[i]->getPort(nameList[1])->info();
                    return info.join(" ");
                }
            }
        }
    } else if (type == "Interface") {
        for (int i=0;i<bridgeList.length();i++) {
            if (bridgeList[i]->getName() == nameList[0]) {
                QStringList info = bridgeList[i]->getPort(nameList[1])->getInterface(nameList[2])->info();
                return info.join(" ");
            }
        }
    }
    return "No description";
}

void Controller::addBridge(QString bridgeName) {
    ps->addBridge(bridgeName);
}

void Controller::deleteBridge(QString bridgeName) {
    ps->deleteBridge(bridgeName);
}

void Controller::addPort(QString bridgeName, QString portName) {
    ps->addPort(bridgeName, portName);
}

void Controller::deletePort(QString bridgeName, QString portName) {
    ps->deletePort(bridgeName, portName);
}

void Controller::startOvs() {
    ps->startOvs();
}

void Controller::refreshOvs() {
    qDebug()<<"INFO: refresh";
    bridgeList.clear();
    emit clearAll();
    ps->listStatus();
}

void Controller::execErrorFound(QString err) {
    qDebug()<<"ERROR:"<<err;
    emit execErrorConfirmed(err);
}

void Controller::readNVO3(QString type) {
    if (type == "BrVN") {
        QJsonObject result = ps->readJson("./data/brvnid");
        QJsonObject obj = result["brvnid"].toObject();
        QStringList keys = obj.keys();
        for (int i=0;i<keys.length();i++) {
            QStringList row;
            row<<keys[i];
            row<<obj[keys[i]].toString();
            emit nvo3DataConfirmed(row);
        }
    } else if (type == "LocalVNIP") {
        QJsonObject result = ps->readJson("./data/vnidip");
        QJsonObject obj = result["vnidip"].toObject();
        QStringList keys = obj.keys();
        for (int i=0;i<keys.length();i++) {
            QStringList row;
            row<<keys[i];
            row<<obj[keys[i]].toArray()[0].toString();
            emit nvo3DataConfirmed(row);
        }
    } else if (type == "VNIP") {
        //read Local VNIP
        QJsonObject result = ps->readJson("./data/vnidip");
        QJsonObject obj = result["vnidip"].toObject();
        QStringList keys = obj.keys();
        for (int i=0;i<keys.length();i++) {
            QStringList row;
            row<<keys[i];
            row<<obj[keys[i]].toArray()[0].toString();
            emit nvo3DataConfirmed(row);
        }
        //read Global VNIP
        result = ps->readJson("./data/ip_write");
        obj = result["vnidip"].toObject();
        keys = obj.keys();
        for (int i=0;i<keys.length();i++) {
            QJsonArray array = obj[keys[i]].toArray();
            for (int j=0;j<array.size();j++) {
                QStringList row;
                row<<keys[i];
                row<<array[j].toString();
                emit nvo3DataConfirmed(row);
            }
        }
    } else if (type == "VNMACIP") {
        QJsonObject vnidMac = ps->readJson("./data/mac_write");
        QJsonObject macIp = ps->readJson("./data/mac_ip");
        QJsonObject vnidMacObj = vnidMac["vnidmac"].toObject();
        QJsonObject macIpObj = macIp["macip"].toObject();
        QStringList keys = vnidMacObj.keys();
        QStringList ips = macIpObj.keys();
        for (int i=0;i<keys.length();i++) {
            QStringList row;
            row<<keys[i];
            QString mac = vnidMacObj[keys[i]].toArray()[0].toString();
            row<<mac;
            for (int j=0;j<ips.length();j++){
                if (macIpObj[ips[j]].toArray()[0].toString() == mac) {
                    row<<ips[j];
                    emit nvo3DataConfirmed(row);
                    break;
                }
            }
        }
    } else if (type == "NVE") {
        QJsonObject result = ps->readJson("./data/test_encap_write");
        QJsonObject obj = result["ipencap"].toObject();
        QStringList keys = obj.keys();
        for (int i=0;i<keys.length();i++) {
            QStringList row;
            row<<keys[i];
            row<<obj[keys[i]].toArray()[0].toString();
            emit nvo3DataConfirmed(row);
        }
    }
}

void Controller::setInterface(QString interfaceName, QString type, QStringList options) {
    if (interfaceName.length() == 0) return;
    ps->setInterface(interfaceName, type, options);
}
