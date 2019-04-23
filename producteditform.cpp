#include "producteditform.h"
#include "ui_edithandbookform.h"
#include "product.h"

ProductEditForm::ProductEditForm(QWidget *parent) : EditHandbookForm(parent)
{
    productSearch = new ProductSearchPresenter(this);
    searchForm = new ProductSearchForm(this);
    productSearch->setView(searchForm);
    connect(productSearch, &ProductSearchPresenter::done, this, &ProductEditForm::onSearchDone);

    searchProductButton.setText("Найти изделие");
    connect (&searchProductButton, &QPushButton::clicked, this, &ProductEditForm::onProductSearch);
    codeLabel.setText("Код изделия");
    getUi()->verticalLayout->addWidget(&codeLabel);
    getUi()->verticalLayout->addWidget(&code);
    getUi()->handbook->setVisible(false);
    getUi()->horizontalLayout_3->addWidget(&searchProductButton);

}

void ProductEditForm::setMode(FormMode mode)
{
    EditHandbookForm::setMode(mode);
    getUi()->handbook->setVisible(false);
    if (mode == Adding)
    {
        searchProductButton.setVisible(false);
        return;
    }
    searchProductButton.setVisible(true);
    clearFieldsOnAdd();
    getUi()->id->clear();
}


void ProductEditForm::fillHandbookFields(Handbook** h)
{
    auto hb = new Product();
    hb->id = getUi()->id->text().toInt();
    hb->name = getUi()->name->text();
    hb->code = code.text();

    *h = hb;
}

void ProductEditForm::clearFieldsOnAdd()
{
    EditHandbookForm::clearFieldsOnAdd();
    code.clear();
}

void ProductEditForm::fillFieldsOnEdit(int index)
{
    EditHandbookForm::fillFieldsOnEdit(index);

    auto hb = getHandbooks();
    auto product = (Product*)hb->at(index);
    code.setText(product->code);
}

void ProductEditForm::onProductSearch()
{
    productSearch->getView()->showWindow();
}

void ProductEditForm::onSearchDone()
{
    auto product = productSearch->getProduct();
    if (product->id  == 0)
        return;

    getUi()->id->setText(QString::number(product->id));
    getUi()->name->setText(product->name);
    code.setText(product->code);
    productSearch->getView()->closeWindow();
}
