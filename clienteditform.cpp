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
    if (index == -1)
        return;

    EditHandbookForm::on_handbook_currentIndexChanged(index);

    auto handbooks = getHandbooks();
    auto client = (Client*)(*handbooks)[index];
    phone.setText(client->phone);
    address.setText(client->address);
    contact.setText(client->person);
}

void ClientEditForm::fillHandbookFields(Handbook** h)
{
    auto hb = new Client();
    hb->id = getUi()->id->text().toInt();
    hb->name = getUi()->name->text();
    hb->address = address.text();
    hb->person = contact.text();
    hb->phone = phone.text();

    *h = hb;
}

void ClientEditForm::clearFieldsOnAdd()
{
    EditHandbookForm::clearFieldsOnAdd();
    phone.clear();
    address.clear();
    contact.clear();
}
