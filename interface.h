#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
class Interface : public QObject
{
    Q_OBJECT
public:
    explicit Interface(QString name, QObject *parent = 0);
    QString getName();
    void setType(QString type);
    QString getType();
    QStringList info();
    void setOptions(QString,QString);
signals:

public slots:
private:
    QString _name;
    QString _type;
    QMap<QString, QString> _options;
};

#endif // INTERFACE_H
