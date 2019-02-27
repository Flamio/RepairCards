#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QHBoxLayout>
#include "imainview.h"

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

private slots:

    void on_comboBox_activated(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::MainWindow *ui;

    QHash<int,Client> clients;
    QVector<MethodGui> combos;
    QVector<Handbook> methods;
};

#endif // MAINWINDOW_H
