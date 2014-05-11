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

QString Interface::getKey() {
    return _options["key"];
}

QStringList Interface::info() {
    QStringList sl;
    if (this->getType().length())
        sl<<"Type:"<<this->getType();
    QMap<QString,QString>::iterator it = _options.begin();
    QStringList options;
    while (it!=_options.end()) {
        options.append(it.key() + " : " + it.value());
        it++;
    }
    if (options.length() > 0) {
        sl<<"Options:";
        sl.append(options);
    }
    return sl;
}

void Interface::setOptions(QString key, QString value) {
    _options[key] = value;
}
