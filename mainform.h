#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>
#include "imainview.h"
#include <QLineEdit>
#include <QVector>
#include <QTableWidgetItem>

namespace Ui {
class MainForm;
}

class MainForm : public QDialog, public IMainView
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    virtual void setCard(const RepairCard &card, const QVector<CardMethod>& methods);
    virtual void showWindow(){ show();}
    virtual void closeWindow(){}
    virtual IMainView* newDialog();

    void setIsDialog(bool value);
    void hideNavigationPanel();

protected:
    void closeEvent(QCloseEvent *);

signals:
    void edit(int);
    void add();
    void navigation(bool);
    void deleteSignal(int id);

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainForm *ui;

    bool isDialog = false;
    MainForm* dialog = nullptr;
};

#endif // MAINFORM_H
