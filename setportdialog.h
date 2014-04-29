#ifndef SETPORTDIALOG_H
#define SETPORTDIALOG_H

#include <QDialog>

namespace Ui {
class setPortDialog;
}

class setPortDialog : public QDialog
{
    Q_OBJECT
signals:
    void setPort(QString interfaceName, QString type, QStringList options);
public:
    explicit setPortDialog(QString interfaceName,QWidget *parent = 0);
    ~setPortDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::setPortDialog *ui;
    QString _interfaceName;
};

#endif // SETPORTDIALOG_H
