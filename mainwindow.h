#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QHBoxLayout>
#include "imainview.h"
#include "cardmethod.h"

namespace Ui {
class MainWindow;
}

struct MethodGui
{
    QComboBox* combo;
    QLineEdit* edit;
    QHBoxLayout* layout;
};

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setMethods(QVector<Handbook>&);
    virtual void setStates(QVector<Handbook>&);
    virtual void setRepairers(QVector<Handbook>&);
    virtual void setClients(QHash<int,Client>&);
    virtual void setMode(MainViewMode);
    virtual void showInfo(QString);
    virtual void setCard(const RepairCard&, const QVector<CardMethod>&);
    virtual void setProduct(const Handbook& product);

signals:
    void barCodeFinish(QString barcode);
    void addSignal(const RepairCard& card, const QVector<CardMethod>& methods);
    void newCard();
    void cancelAdding();

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
    Ui::MainWindow *ui;
    QHash<int,Client> clients;
    QVector<MethodGui> combos;
    QVector<Handbook> methods;
    RepairCard creatingCard;
    MainViewMode mode;
    void addGuiMethodsItem();
};

#endif // MAINWINDOW_H
