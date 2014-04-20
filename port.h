#ifndef PORT_H
#define PORT_H

#include <QObject>

class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QString,QObject *parent = 0);
    QString getName();
signals:

public slots:

private:
    QString _name;

};

#endif // PORT_H
