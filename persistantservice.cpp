#include "persistantservice.h"
#include <QDebug>
#include <iostream>
#include <cstdlib>
PersistantService::PersistantService(QObject *parent) :
    QObject(parent)
{
    process = new QProcess;
}
void PersistantService::run(){
    QString query("");
    QStringList arguments;
    arguments << "--help";
    connect(process,SIGNAL(readyRead()),this,SLOT(readOutput()));
    connect(process,SIGNAL(started()),this,SLOT(onStarted()));
    connect(process,SIGNAL(finished(int)),this,SLOT(onFinished()));
    process->start(query);
    qDebug()<<"connected";
}

void PersistantService::startOvs() {
    //1. Start ovs server
    QString query("gksudo");
    QStringList arguments;
    arguments << "ovsdb-server --pidfile --detach --remote=punix:/usr/local/var/run/openvswitch/db.sock";
    process->waitForFinished();
    connect(process,SIGNAL(finished(int)),this,SLOT(onOvsServerStarted()));
    process->start(query, arguments);
}

void PersistantService::onOvsServerStarted() {
    //2. Start ovs-vswitchd
    qDebug()<<"ovs-server started";
    QString query("gksudo");
    QStringList arguments;
    arguments << "ovs-vswitchd --pidfile --detach";
    process->waitForFinished();
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onOvsServerStarted()));
    connect(process,SIGNAL(finished(int)),this,SLOT(onOvsVswitchdStarted()));
    process->start(query, arguments);
}

void PersistantService::onOvsVswitchdStarted() {
    qDebug()<<"ovs-vswitchd started";
    qDebug()<<"ovs started";
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onOvsVswitchdStarted()));
    emit ovsStarted();
    listBridges();
}

void PersistantService::listBridges() {
    QString query("gksudo");
    QStringList arguments;
    arguments << "ovs-vsctl list-br";
    process->waitForFinished();
    connect(process,SIGNAL(finished(int)),this,SLOT(readBridge()));
    connect(process,SIGNAL(finished(int)),this,SLOT(onListBridgeEnd()));
    qDebug()<<"Start reading brs";
    process->start(query, arguments);
}

void PersistantService::readBridge() {
    QString stdout = process->readAllStandardOutput().trimmed();
    QStringList brList = stdout.split("\n");
    for (int i=0;i<brList.length();i++) {
        emit bridgeFound(brList[i]);
        //qDebug()<<brList[i].trimmed();
    }
    qDebug()<<"Once";
    //qDebug() <<brList;
}

void PersistantService::onListBridgeEnd() {
    disconnect(process,SIGNAL(readyRead()),this,SLOT(readBridge()));
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onListBridgeEnd()));
    qDebug()<<"read bridges end";
}

void PersistantService::listPorts(QString bridgeName, bool hasWaited) {
    if (!hasWaited) {
        tempBridgeList.append(bridgeName);
        if (tempBridgeList.length() > 1) return;
    }
    QString query("gksudo");
    QStringList arguments;
    arguments << QString("ovs-vsctl list-ports %1").arg(bridgeName);
    process->waitForFinished();
    connect(process,SIGNAL(readyRead()),this,SLOT(readPort()));
    connect(process,SIGNAL(finished(int)),this,SLOT(onListPortEnd()));
    process->start(query, arguments);
    qDebug()<<"Starting"<<process->program();
}

void PersistantService::readPort() {
    QString stdout = process->readAllStandardOutput().trimmed();
    QStringList portList = stdout.split("\n");
    for (int i=0;i<portList.length();i++) {
        emit portFound(tempBridgeList[0],portList[i]);
    }
    qDebug() <<portList;
}

void PersistantService::onListPortEnd() {
    disconnect(process,SIGNAL(readyRead()),this,SLOT(readPort()));
    disconnect(process,SIGNAL(finished(int)),this,SLOT(onListPortEnd()));
    tempBridgeList.pop_front();
    if (tempBridgeList.length()) listPorts(tempBridgeList[0], true);
}

void PersistantService::readOutput() {
    QString stdout = process->readAllStandardOutput();
    qDebug() <<stdout;
}
void PersistantService::onStarted() {
    qDebug() << "started";
}
void PersistantService::onFinished() {
    qDebug() << "finished";
}
