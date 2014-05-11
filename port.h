#ifndef PORT_H
#define PORT_H

#include <QObject>
#include <QList>
#include <QStringList>
#include "interface.h"
class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QString,QObject *parent = 0);
    QString getName();
    QString getKey();
    QStringList info();
    void addInterface(QString name);
    Interface * getInterface(QString name);
    Interface * getInterface(int index);

signals:

public slots:

private:
    QString _name;
    QList<Interface *> interfaceList;

};

#endif // PORT_H
