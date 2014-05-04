#include "nvo3dialog.h"
#include "ui_nvo3dialog.h"

Nvo3Dialog::Nvo3Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nvo3Dialog)
{
    ui->setupUi(this);
}

Nvo3Dialog::~Nvo3Dialog()
{
    delete ui;
}

void Nvo3Dialog::setHeaders(QStringList labels) {
    ui->tableWidget->setColumnCount(labels.length());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
}

void Nvo3Dialog::instertData(QStringList dataList) {
    ui->tableWidget->setSortingEnabled(false);
    QTableWidgetItem * w;
    ui->tableWidget->insertRow(0);
    for (int i=0;i<dataList.length();i++) {
        w = new QTableWidgetItem;
        w->setText(dataList[i]);
        ui->tableWidget->setItem(0,i,w);
    }
    ui->tableWidget->sortItems(0);
    ui->tableWidget->setSortingEnabled(true);
}
