#ifndef NVO3DIALOG_H
#define NVO3DIALOG_H

#include <QDialog>
#include <QStringList>
namespace Ui {
class Nvo3Dialog;
}

class Nvo3Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Nvo3Dialog(QWidget *parent = 0);
    ~Nvo3Dialog();
    void setHeaders(QStringList labels);
public slots:
    void instertData(QStringList dataList);

private:
    Ui::Nvo3Dialog *ui;
};

#endif // NVO3DIALOG_H
