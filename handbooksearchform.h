#pragma once

#include <QDialog>
#include <QListWidgetItem>
#include "ihandbooksearchview.h"

namespace Ui {
class HandbookSearchForm;
}

class HandbookSearchForm : public QDialog, public IHandbookSearchView
{
    Q_OBJECT

public:
    explicit HandbookSearchForm(QWidget *parent = 0);
    ~HandbookSearchForm();

    virtual void showWindow() {this->show();}
    virtual void closeWindow() {this->close();}
    void setName(const QString& name);
    virtual void setHandbooks(QVector<Handbook>&);
    virtual void setCallbacks(const HandbookSearchCallbacks& c)
    {
        callbacks = c;
    }

private:
    Ui::HandbookSearchForm *ui;

    QVector<Handbook> handbooks;
    HandbookSearchCallbacks callbacks;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};
