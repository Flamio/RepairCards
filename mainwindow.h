#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imainview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setMethods(QVector<Handbook>&);
    virtual void setStates(QVector<Handbook>&);
    virtual void setRepairers(QVector<Handbook>&);

private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
