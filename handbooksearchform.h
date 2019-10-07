#pragma once

#include <QDialog>
#include <QListWidgetItem>
#include <QCheckBox>
#include "ihandbooksearchview.h"
#include "ui_handbooksearchform.h"

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
    virtual void setOption(const QString& type, const QString& name, const QString& label);
    virtual void setQueryTitle(const QString& title)
    {
        ui->label_2->setText(title);
    }

private:
    Ui::HandbookSearchForm *ui;

    QVector<Handbook> handbooks;
    HandbookSearchCallbacks callbacks;

    QVector<QCheckBox*> checkboxes;

    QMap<QString, std::function<void(const QString& name, const QString& label)>> optionsMap {{"check", [=](const QString& name,  const QString& label)
    {
        auto check = new QCheckBox(this);
        check->setObjectName(name);
        check->setText(label);
        connect(check, &QCheckBox::clicked, [=](bool checked)
        {
            QMap<QString, QVariant> map{{"issued_products", checked}};
            callbacks.searchHandbook(ui->name->text(),  map);
        });
        checkboxes.append(check);
        ui->gridLayout->addWidget(check,3,0);
    } }};

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

};
