#include "addform.h"
#include "ui_addform.h"
#include <QMessageBox>
#include "helper.h"

AddForm::AddForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddForm)
{
    ui->setupUi(this);
}

AddForm::~AddForm()
{
    delete ui;
}

void AddForm::setMethods(QVector<Handbook>& methods)
{
    this->methods = methods;
}

void AddForm::setStates(QVector<Handbook> &states)
{
    ui->state->addItem("", 0);

    foreach (auto state, states)
        ui->state->addItem(state.name, state.id);
}

void AddForm::setRepairers(QVector<Handbook> &repairers)
{
    ui->repairer->addItem("", 0);

    foreach (auto state, repairers)
        ui->repairer->addItem(state.name, state.id);
}

void AddForm::setClients(QHash<int,Client> &clients)
{
    ui->client->addItem("", 0);

    QVector<Client> vector;

    foreach (auto client, clients)
        vector.push_back(client);

    qSort(vector.begin(), vector.end(), [] (const Client& one, const Client& two) { return one.name < two.name;});

    foreach (auto client, vector)
        ui->client->addItem(client.name, client.id);

    this->clients = clients;
}

void AddForm::showInfo(QString info)
{
    QMessageBox msg;
    msg.setText(info);
    msg.exec();
}

void AddForm::setCard(const RepairCard &card)
{
    RepairCard card_;
    creatingCard = card_;
    creatingCard.id = card.id;
    ui->createMonth->clear();
    ui->createYear->clear();

    ui->cardNumber->setText(QString::number(creatingCard.id));
    ui->barCode->setText(creatingCard.barCode);
    ui->client->setCurrentIndex(ui->client->findData(creatingCard.clientId));
    auto hashIndex  = ui->client->currentData();
    auto address = clients[hashIndex.toInt()].address;
    ui->clientAddress->setText(address);

    ui->complains->setText(creatingCard.complaints);
    ui->note->setText(creatingCard.note);
    ui->reason->setText(creatingCard.reason);
    ui->product->setText(creatingCard.productName);
    ui->receiveDate->setDate(creatingCard.receiveFromClientDate);
    ui->returnDate->setDate(creatingCard.returnDate);
    ui->repairer->setCurrentIndex(ui->repairer->findData(creatingCard.repairerId));
    ui->state->setCurrentIndex(ui->state->findData(creatingCard.stateId));
    ui->readyDate->setDate(creatingCard.readyDate);
    ui->repairCost->setValue(creatingCard.costRepair);
    ui->clientCost->setValue(creatingCard.costForClient);

    foreach (MethodGui item, combos) {
        delete item.combo;
        delete item.edit;
        combos.pop_back();
    }
}

void AddForm::closeWindow()
{
    close();
}

void AddForm::showWindow()
{
    show();
}

void AddForm::setProduct(const Handbook& product)
{
    ui->product->setText(product.name);
    creatingCard.productId = product.id;
}

void AddForm::on_client_activated(int index)
{
    auto hashIndex  = ui->client->itemData(index);
    auto address = clients[hashIndex.toInt()].address;
    ui->clientAddress->setText(address);
}

void AddForm::on_pushButton_4_clicked()
{
    QComboBox* combo = new QComboBox(this);
    combo->addItem("", 0);

    foreach (auto method, methods) {
        combo->addItem(method.name, method.id);
    }

    auto lineEdit = new QLineEdit(this);

    ui->verticalLayout_8->addWidget(combo);
    ui->verticalLayout_9->addWidget(lineEdit);

    MethodGui mgui;
    mgui.combo = combo;
    mgui.combo->setEditable(true);
    mgui.edit = lineEdit;
    combos.push_back(mgui);
}

void AddForm::on_pushButton_5_clicked()
{
    if (combos.count() == 0)
        return;

    MethodGui item = combos.last();
    delete item.combo;
    delete item.edit;
    combos.pop_back();
}

void AddForm::on_pushButton_10_clicked()
{
}

void AddForm::on_pushButton_11_clicked()
{
    if (ui->barCode->text().count() != barCodeLenght)
    {
        showInfo("Неправильный штрихкод!");
        return;
    }

    if (ui->createMonth->text().toInt() < 1 || ui->createMonth->text().toInt() > 12)
    {
        showInfo("Неправильный месяц выпуска изделия!");
        return;
    }

    if (ui->client->currentData().toInt() == 0)
    {
        showInfo("Не выбран клиент!");
        return;
    }

    if (creatingCard.productId == 0)
    {
        showInfo("Не найдено изделие!");
        return;
    }

    if ( ui->repairer->currentData().toInt() == 0)
    {
        showInfo("Не выбран ремонтник!");
        return;
    }

    if ( ui->state->currentData().toInt() == 0)
    {
        showInfo("Не выбрано состояние!");
        return;
    }
    if (ui->clientCost->value() != 0 && ui->repairCost->value() != 0 ||
            ui->clientCost->value() == 0 && ui->repairCost->value() == 0 )
    {
        showInfo("Должно быть заполнена цена для клиента или цена ремонта!");
        return;
    }

    creatingCard.barCode = ui->barCode->text();
    creatingCard.clientId = ui->client->currentData().toInt();
    creatingCard.complaints = ui->complains->toPlainText();
    creatingCard.id = ui->cardNumber->text().toInt();
    creatingCard.note = ui->note->toPlainText();
    creatingCard.readyDate = ui->readyDate->date();
    creatingCard.reason = ui->reason->toPlainText();
    creatingCard.receiveFromClientDate = ui->receiveDate->date();
    creatingCard.repairerId = ui->repairer->currentData().toInt();
    creatingCard.returnDate = ui->returnDate->date();
    creatingCard.stateId = ui->state->currentData().toInt();
    creatingCard.costForClient = ui->clientCost->value();
    creatingCard.costRepair = ui->repairCost->value();

    QVector<CardMethod> cardMethods;
    foreach (MethodGui element, combos)
    {
        if (element.combo->currentData().toInt() == 0)
            continue;
        CardMethod cardMethod;
        cardMethod.description = element.edit->text();
        cardMethod.methodId = element.combo->currentData().toInt();
        cardMethod.cardId = creatingCard.id;
        cardMethods.push_back(cardMethod);
    }

    addSignal(creatingCard, cardMethods);
}

void AddForm::on_barCode_textChanged(const QString &arg1)
{
    if (arg1.count() != barCodeLenght)
        return;

    barCodeFinish(arg1);

    auto data = Helper::ParseBarcode(arg1);

    ui->createMonth->setText(data.month);
    ui->createYear->setText("20"+data.year);
}

void AddForm::on_pushButton_12_clicked()
{
    cancelAdding();
}
