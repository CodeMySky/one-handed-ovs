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
    QMap<QString, QString> attrMap;
    if (ui->keyLineEdit->text().length()>0) {
        attrMap["options:key"] = ui->keyLineEdit->text();
    }
    if (ui->ipLineEdit->text().length()>0) {
        attrMap["options:remote_ip"] = ui->ipLineEdit->text();
    }
    attrMap["ingress_policing_rate"] =  QString::number(ui->ingressPolicingRateSpinBox->value());
    attrMap["ingress_policing_burst"] =  QString::number(ui->ingressPolicingBurstSpinBox->value());
    attrMap["type"] = ui->typeComboBox->currentText();
    emit setPort(_interfaceName, attrMap);
    this->close();
}

void setPortDialog::echoPortAttr(QString key, QString value) {
    if (key == "key") {
        ui->keyLineEdit->setText(value);
    } else if (key == "remote_ip") {
        ui->ipLineEdit->setText(value);
    } else if (key == "ingress_policing_rate") {
        ui->ingressPolicingRateSpinBox->setValue(value.toInt());
    } else if (key == "ingress_policing_burst") {
        ui->ingressPolicingBurstSpinBox->setValue(value.toInt());
    }
}

void setPortDialog::on_cancelButton_clicked()
{
    this->close();
}
