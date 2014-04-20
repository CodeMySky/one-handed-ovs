#include "persistantservice.h"
#include <QDebug>
#include <iostream>
#include <cstdlib>
PersistantService::PersistantService(QObject *parent) :
    QObject(parent)
{
    process = new QProcess;
    watchDog = new QTimer;
    connect(process,SIGNAL(started()),this,SLOT(wakeWatchDog()));
    connect(process,SIGNAL(finished(int)),watchDog,SLOT(stop()));
    connect(watchDog,SIGNAL(timeout()),this,SLOT(killProcess()));

}

void PersistantService::wakeWatchDog() {
    watchDog->start(300);
}

void PersistantService::killProcess() {
    process->kill();
    qDebug()<<"ERROR: excution timeout";
    emit execErrorFound("Excution timeout!");
}

void PersistantService::onNormalProcessEnd() {
    disconnect(process, SIGNAL(finished(int)), this, SLOT(onNormalProcessEnd()));
    QString stderr = process->readAllStandardError().trimmed();
    if (stderr.length()) {
        emit execErrorFound(stderr);
    } else {
        emit needRefresh();
    }
}

void PersistantService::startOvs() {
    //1. Start ovs server
    QString query("gksudo");
    QStringList arguments;
    arguments << "ovsdb-server --pidfile --detach --remote=punix:/usr/local/var/run/openvswitch/db.sock";
    connect(process,SIGNAL(finished(int)),this,SLOT(onOvsServerStarted()));
    process->start(query, arguments);
    process->waitForFinished();
}

void PersistantService::onOvsServerStarted() {
    //2. Start ovs-vswitchd
    qDebug()<<"INFO: ovs-server started";
    QString query("gksudo");
    QStringList arguments;
    arguments << "ovs-vswitchd --pidfile --detach";
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onOvsServerStarted()));
    connect(process,SIGNAL(finished(int)),this,SLOT(onOvsVswitchdStarted()));
    process->start(query, arguments);
    process->waitForFinished();
}

void PersistantService::onOvsVswitchdStarted() {
    qDebug()<<"INFO: ovs-vswitchd started";
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onOvsVswitchdStarted()));
    emit ovsStarted();
}

void PersistantService::listStatus() {
    QString query("gksudo");
    QStringList arguments;
    arguments << "ovs-vsctl show";
    process->waitForFinished();
    connect(process,SIGNAL(finished(int)),this,SLOT(readStatus()));
    connect(process,SIGNAL(finished(int)),this,SLOT(onListStatusEnd()));
    process->start(query, arguments);
}

void PersistantService::readStatus() {
    QString stdout = process->readAllStandardOutput().trimmed();
    QStringList brList = stdout.split("\n");
    for (int i=0;i<brList.length();i++) {
        QRegExp exp("([\\w:]+) ([\\w.\"_{}=]+)"), quot("\"");
        exp.indexIn(brList[i]);
        QString key = exp.cap(1), value = exp.cap(2);
        if (key == "Bridge" || key == "Port" || key == "Interface") {
            quot.indexIn(value);
            value = value.replace(quot,"");
        }
        if (key == "Bridge") {
            currentBridge = value;
            emit bridgeFound(currentBridge);
        } else if (key == "Port") {
            currentPort = value;
            emit portFound(currentBridge,currentPort);
        } else if (key == "Interface") {
            currentInterface = value;
            emit interfaceFound(currentBridge, currentPort, currentInterface);
        } else if (key == "type:") {
            emit interfaceTypeFound(currentBridge, currentPort, currentInterface, value);
        } else if (key == "options:") {
            QJsonDocument json;
            json = QJsonDocument::fromBinaryData(value.toLocal8Bit());
        }
    }
}

void PersistantService::onListStatusEnd() {
    disconnect(process,SIGNAL(readyRead()),this,SLOT(readStatus()));
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onListStatusEnd()));
}

void PersistantService::addBridge(QString bridgeName) {
    QString query("gksudo");
    QStringList arguments;
    arguments << QString("ovs-vsctl add-br %1").arg(bridgeName);
    qDebug()<<arguments;
    currentBridge = bridgeName;
    connect(process, SIGNAL(finished(int)), this, SLOT(onNormalProcessEnd()));
    process->start(query, arguments);
    process->waitForFinished();
}

void PersistantService::deleteBridge(QString bridgeName) {
    QString query("gksudo");
    QStringList arguments;
    arguments << QString("ovs-vsctl del-br %1").arg(bridgeName);
    qDebug()<<arguments;
    currentBridge = bridgeName;
    connect(process, SIGNAL(finished(int)), this, SLOT(onNormalProcessEnd()));
    process->start(query, arguments);
    process->waitForFinished();
}

void PersistantService::addPort(QString bridgeName, QString portName) {
    QString query("gksudo");
    QStringList arguments;
    arguments << QString("ovs-vsctl add-port %1 %2").arg(bridgeName).arg(portName);
    qDebug()<<arguments;
    currentBridge = bridgeName;
    connect(process, SIGNAL(finished(int)), this, SLOT(onNormalProcessEnd()));
    process->start(query, arguments);
    process->waitForFinished();
}

void PersistantService::deletePort(QString bridgeName, QString portName) {
    QString query("gksudo");
    QStringList arguments;
    arguments << QString("ovs-vsctl del-port %1 %2").arg(bridgeName).arg(portName);
    qDebug()<<arguments;
    currentBridge = bridgeName;
    connect(process, SIGNAL(finished(int)), this, SLOT(onNormalProcessEnd()));
    process->start(query, arguments);
    process->waitForFinished();
}

