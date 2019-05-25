#include "productsearchform.h"
#include "ui_productsearchform.h"

ProductSearchForm::ProductSearchForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductSearchForm)
{
    ui->setupUi(this);
}

ProductSearchForm::~ProductSearchForm()
{
    delete ui;
}

void ProductSearchForm::setName(const QString &name)
{
    ui->name->setText(name);
}

void ProductSearchForm::setProducts(QVector<Product> &products)
{
    this->products = products;
    ui->listWidget->clear();

    for (auto product : products)
        ui->listWidget->addItem(product.name);
}

void ProductSearchForm::on_pushButton_2_clicked()
{
    this->close();
}

void ProductSearchForm::on_pushButton_clicked()
{
    auto index = ui->listWidget->currentIndex().row();
    if (index == -1)
        return;
    auto product = products.at(index);
    ui->listWidget->clear();
    ui->name->clear();
    emit done(product);
}

void ProductSearchForm::on_name_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->listWidget->clear();
        return;
    }
    emit searchProduct(arg1);
}

void ProductSearchForm::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto index = ui->listWidget->row(item);
    emit done(products.at(index));
}
