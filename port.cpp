#include "port.h"

Port::Port(QString p,QObject *parent) :
    QObject(parent),_name(p)
{
}

QString Port::getName() {
    return _name;
}

QStringList Port::info() {
    QStringList sl;
    if (interfaceList.length() == 0) {
        sl<<"No interface";
    }
    for (int i=0;i<interfaceList.length();i++) {
        sl<<QString("接口类型:%1").arg(interfaceList[i]->getType());
    }
    return sl;
}

void Port::addInterface(QString name) {
    Interface *in = new Interface(name);
    interfaceList.append(in);
}

Interface * Port::getInterface(QString name) {
    for (int i=0;i<interfaceList.length();i++) {
        if (interfaceList[i]->getName() == name) {
            return interfaceList[i];
        }
    }
    return NULL;
}
Interface * Port::getInterface(int index) {
    if (index >=0 && index <= interfaceList.length()) {
        return interfaceList[index];
    }
    return NULL;
}

QString Port::getKey() {
    return interfaceList[0]->getKey();
}
