#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QHBoxLayout>
#include "iaddview.h"
#include "cardmethod.h"

namespace Ui {
class AddForm;
}

struct MethodGui
{
    QComboBox* combo;
    QLineEdit* edit;
};

class AddForm : public QMainWindow, public IAddView
{
    Q_OBJECT

public:
    explicit AddForm(QWidget *parent = 0);
    ~AddForm();

    void setMethods(QVector<Handbook>&);
    virtual void setStates(QVector<Handbook>&);
    virtual void setRepairers(QVector<Handbook>&);
    virtual void setClients(QHash<int,Client>&);    
    virtual void showInfo(QString);
    virtual void setProduct(const Handbook& product);
    virtual void showWindow();
    virtual void setCard(const RepairCard &card);
    virtual void closeWindow();

signals:
    void barCodeFinish(QString barcode);
    void cancelAdding();
    void addSignal(const RepairCard& card, const QVector<CardMethod>& methods);

private slots:

    void on_client_activated(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_barCode_textChanged(const QString &arg1);

    void on_pushButton_12_clicked();

private:
    const int barCodeLenght = 17;
    Ui::AddForm *ui;
    QHash<int,Client> clients;
    QVector<MethodGui> combos;
    QVector<Handbook> methods;
    RepairCard creatingCard;
    void addGuiMethodsItem();
};

#endif // MAINWINDOW_H
