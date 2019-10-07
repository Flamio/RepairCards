#include "producteditform.h"
#include "ui_edithandbookform.h"
#include "product.h"

ProductEditForm::ProductEditForm(QWidget *parent) : EditHandbookForm(parent)
{
    searchForm = new HandbookSearchForm(this);

    HandbookSearchCallbacks c;
    c.searchHandbook = [=](const QString& name, QMap<QString, QVariant>)
    {
        (*products) = this->getProductsByNameFunc(name);
        QVector<Handbook> h;

        for (auto p : *products)
            h.append(p);

        searchForm->setHandbooks(h);
    };
    c.done = [=] (int index)
    {
        auto product = (*products)[index];
        getUi()->id->setText(QString::number(product.id));
        getUi()->name->setText(product.name);
        isOwenCheck.setChecked(product.isOwen);
        code.setText(product.code);
        searchForm->closeWindow();
    };
    searchForm->setCallbacks(c);


    connect(&isOwenCheck, &QCheckBox::stateChanged, this, &ProductEditForm::onOwenCheck);

    searchProductButton.setText("Найти изделие");
    connect (&searchProductButton, &QPushButton::clicked, this, &ProductEditForm::onProductSearch);
    codeLabel.setText("Код изделия");
    isOwenCheck.setText("OWEN");
    isOwenCheck.setCheckState(Qt::CheckState::Checked);
    getUi()->verticalLayout->addWidget(&isOwenCheck);
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

void ProductEditForm::setProduct(Product &p)
{
    getUi()->id->setText(QString::number(p.id));
    getUi()->name->setText(p.name);
    isOwenCheck.setChecked(p.isOwen);
    code.setText(p.code);
}

void ProductEditForm::fillHandbookFields(Handbook** h)
{
    auto hb = new Product();
    hb->id = getUi()->id->text().toInt();
    hb->name = getUi()->name->text();
    hb->code = code.text();
    hb->isOwen = isOwenCheck.checkState();
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

void ProductEditForm::setGetProductsByNameFunc(const std::function<QVector<Product>(const QString&)> &value)
{
    getProductsByNameFunc = value;
}

void ProductEditForm::onProductSearch()
{
    searchForm->showWindow();
}

void ProductEditForm::onOwenCheck(bool checked)
{
    code.setVisible(checked);
    codeLabel.setVisible(checked);
    if (!checked)
        code.clear();
}
