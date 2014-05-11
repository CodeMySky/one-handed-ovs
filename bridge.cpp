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

Port * Bridge::getPort(int index) {
    if (index >=0 && index <= portList.length())
        return portList[index];
    else return NULL;
}

QStringList Bridge::info() {
    QStringList sl;
    if (portList.length() == 0) {
        sl<<"暂无端口";
    } else {
        sl<<QString("VN:%1").arg(portList[0]->getKey());
    }
    return sl;
}
