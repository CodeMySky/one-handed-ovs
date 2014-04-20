#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include "controller.h"
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
    void treeClicked();
    void on_startOvsBtn_clicked();
    void on_addBridgeBtn_clicked();
    void on_deleteBtn_clicked();

};

#endif // MAINWINDOW_H
