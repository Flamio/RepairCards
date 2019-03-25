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
    getUi()->verticalLayout->addWidget(&addressLabel);
    getUi()->verticalLayout->addWidget(&address);
}

void ClientEditForm::on_handbook_currentIndexChanged(int index)
{
    EditHandbookForm::on_handbook_currentIndexChanged(index);

    auto handbooks = getHandbooks();

    auto client = (Client*)handbooks[index];
    phone.setText(client->phone);
    address.setText(client->address);
    contact.setText(client->person);
}
