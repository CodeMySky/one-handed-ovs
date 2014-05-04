#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QContextMenuEvent>
#include "controller.h"
#include "setportdialog.h"
#include "nvo3dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Controller *controller;
    QList<QTreeWidgetItem *> bridgeItemList;
    QList<QList<QTreeWidgetItem *> * > portItemList;
private slots:
    void clearAll();
    void echoBridge(QString bridgeName);
    void echoPort(int index, QString bridgeName);
    void echoInterface(int brIndex, int portIndex, QString interfaceName);
    void echoError(QString);
    void echoKey(int, QString);
    void treeClicked();
    void showMenu();
    void on_action_Br_VN_triggered();
    void on_action_Local_VN_IP_triggered();
    void on_action_VN_IP_triggered();
    void on_action_VN_MAC_IP_triggered();
    void on_action_NVE_triggered();
    void on_action_add_br_triggered();
    void deleteSth();
    void addPort();
    void setInterface();
};

#endif // MAINWINDOW_H
