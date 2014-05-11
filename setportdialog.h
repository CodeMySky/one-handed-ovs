#ifndef SETPORTDIALOG_H
#define SETPORTDIALOG_H

#include <QDialog>
#include <QMap>
namespace Ui {
class setPortDialog;
}

class setPortDialog : public QDialog
{
    Q_OBJECT
signals:
    void setPort(QString interfaceName, QMap<QString, QString>);
public:
    explicit setPortDialog(QString interfaceName,QWidget *parent = 0);
    ~setPortDialog();

public slots:
    void echoPortAttr(QString key, QString value);

private slots:
    void on_pushButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::setPortDialog *ui;
    QString _interfaceName;
};

#endif // SETPORTDIALOG_H
