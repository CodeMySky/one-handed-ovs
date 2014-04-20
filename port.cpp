#include "port.h"

Port::Port(QString p,QObject *parent) :
    QObject(parent),_name(p)
{
}

QString Port::getName() {
    return _name;
}
