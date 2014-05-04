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
            if (key == "key") emit keyConfirmed(i, value);
        }
    }
}

QString Controller::getInfo(QString type, int brIndex, int portIndex, int interfaceIndex) {
    if (type == "Bridge" && brIndex >= 0 && brIndex <= bridgeList.length()) {
        QStringList info = bridgeList[brIndex]->info();
        return info.join(" ");
    } else if (type == "Port" && brIndex >= 0 && brIndex <= bridgeList.length() && portIndex >= 0) {
        QStringList info = bridgeList[brIndex]->getPort(portIndex)->info();
        return info.join(" ");
    } else if (type == "Interface" && brIndex >= 0 && brIndex <= bridgeList.length() && portIndex >= 0 && interfaceIndex >= 0) {
        QStringList info = bridgeList[brIndex]->getPort(portIndex)->getInterface(interfaceIndex)->info();
        return info.join(" ");
    }
    return "No description";
}

void Controller::addBridge(QString bridgeName) {
    ps->addBridge(bridgeName);
}

void Controller::deleteBridge(int brIndex) {
    if (brIndex >=0 && brIndex < bridgeList.length()) {
        QString brName = bridgeList[brIndex]->getName();
        ps->deleteBridge(brName);
    }
}

void Controller::addPort(int brIndex, QString portName) {
    if (brIndex >=0 && brIndex < bridgeList.length()) {
        ps->addPort(bridgeList[brIndex]->getName(), portName);
    }
}

void Controller::deletePort(int brIndex, int portIndex) {
    if (brIndex >= 0 && brIndex < bridgeList.length()) {
        Bridge * br = bridgeList[brIndex];
        QString brName = br->getName();
        QString portName = br->getPort(portIndex)->getName();
        ps->deletePort(brName, portName);
    }
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
        QJsonObject localMac = ps->readJson("./data/vnid-mac");
        QJsonObject localIp = ps->readJson("./data/vnidip");
        QJsonObject localMacObj = localMac["vnidmac"].toObject();
        QJsonObject localIpObj = localIp["vnidip"].toObject();
        keys = localMacObj.keys();
        for (int i=0;i<keys.length();i++) {
            QString ip = localIpObj[keys[i]].toArray()[0].toString();
            if (!ip.isEmpty()) {
                QStringList row;
                row << keys[i];
                row << localMacObj[keys[i]].toArray()[0].toString();
                row << ip;
                emit nvo3DataConfirmed(row);
                break;
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
