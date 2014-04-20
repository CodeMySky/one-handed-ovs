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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::echoBridge(QString bridgeName) {
    QTreeWidgetItem *bridgeItem = new QTreeWidgetItem(ui->treeWidget,QStringList(bridgeName));
    bridgeItemList.append(bridgeItem);
    QList<QTreeWidgetItem *> *itemList = new QList<QTreeWidgetItem *>;
    portItemList.append(itemList);
}

void MainWindow::echoPort(int index, QString portName) {
    QTreeWidgetItem *portItem = new QTreeWidgetItem(bridgeItemList[index], QStringList(portName));
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(treeClicked(QTreeWidgetItem*,int)));
    portItemList[index]->append(portItem);
    qDebug()<<"inserting port"<<portName;
}

void MainWindow::echoInterface(int brIndex,int portIndex, QString interfaceName) {
    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(portItemList[brIndex]->at(portIndex), QStringList(interfaceName));
    qDebug()<<"inserting interface"<<interfaceName;
}

void MainWindow::on_startOvsBtn_clicked()
{
    qDebug()<<"Starting Ovs";
    controller->startOvs();
}

void MainWindow::treeClicked(QTreeWidgetItem *item, int col) {
    if (item->parent()) {
        //controller->
    }
}
