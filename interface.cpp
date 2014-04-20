#include "interface.h"

Interface::Interface(QString name,QObject *parent) :
    QObject(parent), _name(name)
{
}

QString Interface::getName() {
    return _name;
}

void Interface::setType(QString type) {
    _type = type;
}

QString Interface::getType() {
    return _type;
}

QStringList Interface::info() {
    QStringList sl;
    if (this->getType().length())
        sl<<"Type:"<<this->getType();
    return sl;
}
