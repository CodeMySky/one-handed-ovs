#include "bridge.h"

Bridge::Bridge(QString br,QObject *parent) :
    QObject(parent),_name(br)
{
}

void Bridge::addPort(QString port) {
    Port* p = new Port(port);
    ports.append(p);
}

QString Bridge::getName() {
    return _name;
}

int Bridge::indexOf(QString portName){
    for (int i=0;i<ports.length();i++) {
        if (ports[i]->getName() == portName) {
            return i;
        }
    }
    return -1;
}
