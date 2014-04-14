#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTreeWidgetItem>
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

private slots:
    void echoBridge(QString bridgeName);
    void echoPort(int index, QString bridgeName);
    void treeClicked(QTreeWidgetItem* item, int col);
    void on_startOvsBtn_clicked();
};

#endif // MAINWINDOW_H
