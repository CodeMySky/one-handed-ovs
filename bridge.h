#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QList>
#include <QStringList>
#include "port.h"
class Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Bridge(QString br, QObject *parent = 0);
    void addPort(QString);
    QString getName();
    int indexOf(QString portName);
    Port * getPort(QString portName);
    Port * getPort(int index);
    QStringList info();
signals:

public slots:

private:
    QString _name;
    QList<Port*> portList;

};

#endif // BRIDGE_H
