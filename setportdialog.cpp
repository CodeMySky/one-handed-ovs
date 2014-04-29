#include "setportdialog.h"
#include "ui_setportdialog.h"

setPortDialog::setPortDialog(QString interfaceName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setPortDialog),
    _interfaceName(interfaceName)
{
    ui->setupUi(this);
}

setPortDialog::~setPortDialog()
{
    delete ui;
}

void setPortDialog::on_pushButton_clicked()
{
    QStringList options;
    if (ui->keyLineEdit->text().length()>0) {
        options.append("key");
        options.append(ui->keyLineEdit->text());
    }
    if (ui->ipLineEdit->text().length()>0) {
        options.append("remote_ip");
        options.append(ui->ipLineEdit->text());
    }
    emit setPort(_interfaceName,ui->typeComboBox->currentText(),options);
}
