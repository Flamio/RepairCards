#include "productsearchform.h"
#include "ui_productsearchform.h"

ProductSearchForm::ProductSearchForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductSearchForm)
{
    ui->setupUi(this);
    ui->number->setValidator(new QRegExpValidator(QRegExp("^\\d{0,}$"),this));
}

ProductSearchForm::~ProductSearchForm()
{
    delete ui;
}

void ProductSearchForm::setName(const QString &name)
{
    ui->name->setText(name);
}

void ProductSearchForm::on_number_textChanged(const QString &arg1)
{
    emit searchProduct(arg1);
}

void ProductSearchForm::on_pushButton_2_clicked()
{
    this->close();
}

void ProductSearchForm::on_pushButton_clicked()
{
    emit done();
}
