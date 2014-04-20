#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QList>
#include "port.h"
class Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Bridge(QString br, QObject *parent = 0);
    void addPort(QString);
    QString getName();
    int indexOf(QString portName);
signals:

public slots:

private:
    QString _name;
    QList<Port*> ports;

};

#endif // BRIDGE_H
