#include "bridge.h"

Bridge::Bridge(QString br,QObject *parent) :
    QObject(parent),_name(br)
{
}

void Bridge::addPort(QString port) {
    Port* p = new Port(port);
    portList.append(p);
}

QString Bridge::getName() {
    return _name;
}

int Bridge::indexOf(QString portName){
    for (int i=0;i<portList.length();i++) {
        if (portList[i]->getName() == portName) {
            return i;
        }
    }
    return -1;
}

Port * Bridge::getPort(QString portName) {
    for (int i=0;i<portList.length();i++) {
        if (portList[i]->getName() == portName) {
            return portList[i];
        }
    }
    return NULL;
}

QStringList Bridge::info() {
    QStringList sl;
    if (portList.length() == 0) {
        sl<<"No port";
    } else {
        sl<<"Port List:";
    }
    for (int i=0;i<portList.length();i++) {
        sl<<portList[i]->getName();
    }
    return sl;
}
