#include "handbooksearchform.h"
#include "ui_handbooksearchform.h"

HandbookSearchForm::HandbookSearchForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HandbookSearchForm)
{
    ui->setupUi(this);
}

HandbookSearchForm::~HandbookSearchForm()
{
    delete ui;
}

void HandbookSearchForm::setName(const QString &name)
{
    ui->name->setText(name);
}

void HandbookSearchForm::setHandbooks(QVector<Handbook> &h)
{
    this->handbooks = h;
    ui->listWidget->clear();

    for (auto h_ : h)
        ui->listWidget->addItem(h_.name);
}

void HandbookSearchForm::on_pushButton_2_clicked()
{
    this->close();
}

void HandbookSearchForm::on_pushButton_clicked()
{
    auto index = ui->listWidget->currentIndex().row();
    if (index == -1)
        return;

    ui->listWidget->clear();
    ui->name->clear();
    callbacks.done(index);
}

void HandbookSearchForm::on_name_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->listWidget->clear();
        return;
    }
    callbacks.searchHandbook(arg1);
}

void HandbookSearchForm::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto index = ui->listWidget->row(item);
    callbacks.done(index);
}
