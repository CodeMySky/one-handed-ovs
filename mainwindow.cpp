#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controller = new Controller;
    connect(controller,SIGNAL(bridgeConfirmed(QString)),this,SLOT(echoBridge(QString)));
    connect(controller,SIGNAL(portConfirmed(int,QString)),this,SLOT(echoPort(int,QString)));
    connect(controller,SIGNAL(interfaceConfirmed(int,int,QString)),this,SLOT(echoInterface(int,int,QString)));
    connect(controller,SIGNAL(execErrorConfirmed(QString)), this, SLOT(echoError(QString)));
    connect(ui->refreshButton,SIGNAL(clicked()),controller,SLOT(refreshOvs()));
    connect(controller,SIGNAL(clearAll()),this,SLOT(clearAll()));
    connect(ui->treeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearAll() {
    disconnect(ui->treeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeClicked()));
    ui->treeWidget->clear();
    bridgeItemList.clear();
    portItemList.clear();
    connect(ui->treeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeClicked()));
}


void MainWindow::echoBridge(QString bridgeName) {
    QTreeWidgetItem *bridgeItem = new QTreeWidgetItem(ui->treeWidget,QStringList(bridgeName));
    bridgeItemList.append(bridgeItem);
    QList<QTreeWidgetItem *> *itemList = new QList<QTreeWidgetItem *>;
    portItemList.append(itemList);
}

void MainWindow::echoPort(int index, QString portName) {
    QTreeWidgetItem *portItem = new QTreeWidgetItem(bridgeItemList[index], QStringList(portName));
    portItemList[index]->append(portItem);
}

void MainWindow::echoInterface(int brIndex,int portIndex, QString interfaceName) {
    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(portItemList[brIndex]->at(portIndex), QStringList(interfaceName));
}

void MainWindow::treeClicked() {
    QStringList nameList;
    QTreeWidgetItem * item = ui->treeWidget->selectedItems()[0];
    QString info = "No description";
    ui->addPortBtn->setEnabled(false);
    ui->setBtn->setEnabled(false);
    ui->deleteBtn->setEnabled(true);
    if (item->parent()) {
        if (item->parent()->parent()) {
            nameList << item->parent()->parent()->text(0);
            nameList << item->parent()->text(0);
            nameList << item->text(0);
            info = controller->getInfo("Interface", nameList);
            ui->deleteBtn->setEnabled(false);
        } else {
            nameList << item->parent()->text(0) <<item->text(0);
            info = controller->getInfo("Port", nameList);
            ui->setBtn->setEnabled(true);
        }
    } else {
        nameList<<item->text(0);
        info = controller->getInfo("Bridge", nameList);
        ui->addPortBtn->setEnabled(true);
    }
    ui->displayLabel->setText(info);
}

void MainWindow::on_startOvsBtn_clicked()
{
    controller->startOvs();
}


void MainWindow::on_addBridgeBtn_clicked()
{
    QString input = QInputDialog::getText(this,
        "Prompt",
        "Please input bridge name",QLineEdit::Normal,
        "br0",0,0);
    if (input.length() > 0) {
        controller->addBridge(input);
    }
}

void MainWindow::echoError(QString err) {
    QMessageBox msgBox;
    msgBox.setText(err);
    msgBox.exec();
}

void MainWindow::on_deleteBtn_clicked()
{
    QString name = ui->treeWidget->currentItem()[0].text(0);
    QString type;
    QTreeWidgetItem * item = ui->treeWidget->selectedItems()[0];
    if (!item) return;
    if (item->parent()) {
        if (item->parent()->parent()) {
            type = "Interface";
        } else {
            type = "Port";
        }
    } else {
        type = "Bridge";
    }
    QMessageBox msgBox;
    msgBox.setText(QString("You are about to delete a %1. It can not be reverted").arg(type));
    msgBox.setInformativeText("Do you want to continue?");
    msgBox.setStandardButtons( QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();


    if (ret == QMessageBox::Yes) {
        if (type == "Bridge") {
            controller->deleteBridge(name);
        } else if (type == "Port") {
            controller->deletePort(item->parent()->text(0), name);
        } else if (type == "Interface") {

        }
    }
}

void MainWindow::on_addPortBtn_clicked()
{
    QTreeWidgetItem * br = ui->treeWidget->selectedItems()[0];
    if (br && !br->parent()) {
        QString input = QInputDialog::getText(this,
            "Prompt",
            "Please input Port name",QLineEdit::Normal,
            "eth0",0,0);
        if (input.length() > 0) {
            controller->addPort(br->text(0), input);
        }
    }
}
