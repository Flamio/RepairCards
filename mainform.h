#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>
#include "imainview.h"
#include <QLineEdit>
#include <QVector>

namespace Ui {
class MainForm;
}

struct UiMethod
{
    QLineEdit* name = nullptr;
    QLineEdit* description = nullptr;
};

class MainForm : public QDialog, public IMainView
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    virtual void setCard(const RepairCard &card, const QVector<CardMethod>& methods);

signals:
    void add();
    void navigation(bool);

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainForm *ui;

    QVector<UiMethod> methods;
};

#endif // MAINFORM_H