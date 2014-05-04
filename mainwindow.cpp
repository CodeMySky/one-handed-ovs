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
    connect(controller, SIGNAL(keyConfirmed(int, QString)), this, SLOT(echoKey(int, QString)));
    connect(controller,SIGNAL(clearAll()),this,SLOT(clearAll()));
    connect(ui->treeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeClicked()));
    //ui->deleteBtn->setShortcut(QKeySequence::Delete);

    //connect context menu
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu()));

    //connecting menu
    connect(ui->action_start_ovs, SIGNAL(triggered()), controller, SLOT(startOvs()));
    connect(ui->action_refresh, SIGNAL(triggered()), controller, SLOT(refreshOvs()));
    ui->action_refresh->setShortcut(QKeySequence("F5"));

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

void MainWindow::echoKey(int brIndex, QString key) {
    bridgeItemList[brIndex]->setText(0, key);
}

void MainWindow::treeClicked() {
    QTreeWidgetItem * item = ui->treeWidget->selectedItems()[0];
    QString info = "No description";
    if (item->parent()) {
        if (item->parent()->parent()) {
            QTreeWidgetItem * brWidget = item->parent()->parent();
            QTreeWidgetItem * portWidget = item->parent();
            int brIndex = ui->treeWidget->indexOfTopLevelItem(brWidget);
            int portIndex = brWidget->indexOfChild(portWidget);
            int interfaceIndex = portWidget->indexOfChild(item);
            info = controller->getInfo("Interface", brIndex, portIndex, interfaceIndex);
        } else {
            QTreeWidgetItem * brWidget = item->parent();
            int brIndex = ui->treeWidget->indexOfTopLevelItem(brWidget);
            int portIndex = brWidget->indexOfChild(item);
            info = controller->getInfo("Port", brIndex, portIndex);
        }
    } else {
       int brIndex = ui->treeWidget->indexOfTopLevelItem(item);
       info = controller->getInfo("Bridge", brIndex);
    }
    ui->displayLabel->setText(info);
}

void MainWindow::on_action_add_br_triggered()
{
    QString input = QInputDialog::getText(this,
        "提示",
        "请输入网桥名称",QLineEdit::Normal,
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

void MainWindow::deleteSth()
{
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
    msgBox.setText(QString("你将要删除 %1. 此操作不可逆").arg(type));
    msgBox.setInformativeText("是否继续？");
    msgBox.setStandardButtons( QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes) {
        if (type == "Bridge") {
            int brIndex = ui->treeWidget->indexOfTopLevelItem(item);
            controller->deleteBridge(brIndex);
        } else if (type == "Port") {
            QTreeWidgetItem * brWidget = item->parent();
            int brIndex = ui->treeWidget->indexOfTopLevelItem(brWidget);
            int portIndex = brWidget->indexOfChild(item);
            controller->deletePort(brIndex, portIndex);
        } else if (type == "Interface") {

        }
    }
}

void MainWindow::addPort()
{
    QTreeWidgetItem * br = ui->treeWidget->selectedItems()[0];
    if (br && !br->parent()) {
        QString input = QInputDialog::getText(this,
            tr("提示"),
            "请输入Port名称",QLineEdit::Normal,
            "vxa",0,0);
        if (input.length() > 0) {
            controller->addPort(ui->treeWidget->indexOfTopLevelItem(br), input);
        }
    }

}

void MainWindow::setInterface()
{
    QString interfaceName = ui->treeWidget->selectedItems()[0]->text(0);
    setPortDialog *d = new setPortDialog(interfaceName);
    connect(d, SIGNAL(setPort(QString,QString,QStringList)), controller, SLOT(setInterface(QString,QString,QStringList)));
    d->exec();
}

void MainWindow::showMenu() {
    QMenu *menu = new QMenu(this);
    QTreeWidgetItem * currentItem = ui->treeWidget->selectedItems()[0];
    if (currentItem->parent()) {
        if (currentItem->parent()->parent()) {
            //is an interface
            QAction * action_set_interface = menu->addAction(tr("设置接口"));
            connect(action_set_interface, SIGNAL(triggered()), this, SLOT(setInterface()));
        } else {
            //is a port
            QAction * action_delete = menu->addAction(tr("删除"));
            connect(action_delete, SIGNAL(triggered()), this, SLOT(deleteSth()));
            connect(menu, SIGNAL(destroyed()), action_delete, SLOT(deleteLater()));
        }
    } else {
        // it is a bridge
        QAction * action_add_port = menu->addAction(tr("增加端口"));
        connect(action_add_port,SIGNAL(triggered()), this, SLOT(addPort()));
        connect(menu, SIGNAL(destroyed()), action_add_port, SLOT(deleteLater()));
        QAction * action_delete = menu->addAction(tr("删除"));
        connect(action_delete, SIGNAL(triggered()), this, SLOT(deleteSth()));
        connect(menu, SIGNAL(destroyed()), action_delete, SLOT(deleteLater()));
    }
    menu->exec(QCursor::pos());
    delete menu;
}


void MainWindow::on_action_Br_VN_triggered() {
    Nvo3Dialog *d = new Nvo3Dialog;
    QStringList headers ;
    headers <<"网桥名称"<<"网络号";
    d->setHeaders(headers);
    connect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    controller->readNVO3("BrVN");
    d->exec();
    disconnect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    delete d;
}

void MainWindow::on_action_Local_VN_IP_triggered() {
    Nvo3Dialog *d = new Nvo3Dialog;
    QStringList headers ;
    headers<<"网络号"<<"IP";
    d->setHeaders(headers);
    connect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    controller->readNVO3("LocalVNIP");
    disconnect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    d->exec();
    delete d;
}

void MainWindow::on_action_VN_IP_triggered() {
    Nvo3Dialog *d = new Nvo3Dialog;
    QStringList headers ;
    headers<<"网络号"<<"IP";
    d->setHeaders(headers);
    connect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    controller->readNVO3("VNIP");
    disconnect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    d->exec();
    delete d;
}

void MainWindow::on_action_VN_MAC_IP_triggered() {
    Nvo3Dialog *d = new Nvo3Dialog;
    QStringList headers ;
    headers<<"网络号"<<"MAC"<<"IP";
    d->setHeaders(headers);
    connect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    controller->readNVO3("VNMACIP");
    disconnect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    d->exec();
    delete d;
}

void MainWindow::on_action_NVE_triggered(){
    Nvo3Dialog *d = new Nvo3Dialog;
    QStringList headers ;
    headers<<"IP"<<"能力";
    d->setHeaders(headers);
    connect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    controller->readNVO3("NVE");
    disconnect(controller, SIGNAL(nvo3DataConfirmed(QStringList)), d, SLOT(instertData(QStringList)));
    d->exec();
    delete d;
}
