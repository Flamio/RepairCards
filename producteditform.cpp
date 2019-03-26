#include "producteditform.h"
#include "ui_edithandbookform.h"
#include "product.h"

ProductEditForm::ProductEditForm(QWidget *parent) : EditHandbookForm(parent)
{
    codeLabel.setText("Код изделия");
    getUi()->verticalLayout->addWidget(&codeLabel);
    getUi()->verticalLayout->addWidget(&code);
    getUi()->handbook->setEditable(true);
}

void ProductEditForm::on_handbook_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    EditHandbookForm::on_handbook_currentIndexChanged(index);

    auto handbooks = getHandbooks();
    auto product = (Product*)(*handbooks)[index];
    code.setText(product->code);
}
