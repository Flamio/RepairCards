#include "clienteditform.h"
#include "ui_edithandbookform.h"
#include "client.h"

ClientEditForm::ClientEditForm(QWidget *parent) : EditHandbookForm(parent)
{
    phoneLabel.setText("Телефон");
    getUi()->verticalLayout->addWidget(&phoneLabel);
    getUi()->verticalLayout->addWidget(&phone);
    contactLabel.setText("Контактное лицо");
    getUi()->verticalLayout->addWidget(&contactLabel);
    getUi()->verticalLayout->addWidget(&contact);
    addressLabel.setText("Адрес");
}

void ClientEditForm::on_handbook_currentIndexChanged(int index)
{
    EditHandbookForm::on_handbook_currentIndexChanged(index);

    auto h = getHandbooks();
    int a = 0;
}
