#include "addform.h"
#include "ui_addform.h"
#include <QMessageBox>
#include "helper.h"

AddForm::AddForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddForm)
{
    ui->setupUi(this);

    dateOnReturnDate = new DateOnDoubleClick(ui->returnDate);
    dateOnReadyDate = new DateOnDoubleClick(ui->readyDate);
    dateOnReceive2Date = new DateOnDoubleClick(ui->receiveDate2);
    dateOnReceiveDate = new DateOnDoubleClick(ui->receiveDate);
    dateOnSendDate = new DateOnDoubleClick(ui->sendDate);

    ui->barCode->setValidator(new QRegExpValidator(QRegExp("^\\d{17}$"),this));
}

AddForm::~AddForm()
{
    delete ui;
    delete dateOnReturnDate;
    delete dateOnReadyDate;
    delete dateOnReceive2Date;
    delete dateOnReceiveDate;
    delete dateOnSendDate;
}

void AddForm::setMethods(QVector<Handbook*>& methods)
{
    this->methods = methods;
}

void AddForm::setStates(QVector<Handbook*> &states)
{
    this->states = states;
}

void AddForm::setRepairers(QVector<Handbook*> &repairers)
{
    ui->repairer->clear();
    ui->repairer->addItem("", 0);

    foreach (auto state, repairers)
        ui->repairer->addItem(state->name, state->id);
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

void AddForm::setCard(const RepairCard &card, QVector<CardMethod>* methods)
{
    if (mode == Adding)
    {
        RepairCard card_;
        creatingCard = card_;
        creatingCard.id = card.id;
        ui->createMonth->clear();
        ui->createYear->clear();
    }
    else
        creatingCard = card;

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
    ui->receiveDate->setText(creatingCard.receiveFromClientDate.toString("dd.MM.yyyy"));
    ui->returnDate->setText(creatingCard.returnDate.toString("dd.MM.yyyy"));
    ui->repairer->setCurrentIndex(ui->repairer->findData(creatingCard.repairerId));
    showState(creatingCard.stateId);
    ui->readyDate->setText(creatingCard.readyDate.toString("dd.MM.yyyy"));
    ui->repairCost->setValue(creatingCard.costRepair);
    ui->clientCost->setValue(creatingCard.costForClient);
    ui->sendDate->setText(creatingCard.sendDate.toString("dd.MM.yyyy"));
    ui->receiveDate2->setText(creatingCard.receiveFromFactoryDate.toString("dd.MM.yyyy"));

    foreach (MethodGui item, combos) {
        delete item.combo;
        delete item.edit;
        combos.pop_back();
    }

    if (methods == nullptr)
        return;

    for (auto method = methods->begin(); method != methods->end(); method++)
    {
        QComboBox* combo = new QComboBox(this);
        combo->addItem("", 0);

        foreach (auto method_, this->methods) {
            combo->addItem(method_->name, method_->id);
        }

        combo->setCurrentIndex(combo->findData(method->methodId));

        auto lineEdit = new QLineEdit(this);

        auto font = lineEdit->font();
        font.setPointSize(Helper::getFontSize());
        combo->setFont(font);
        lineEdit->setFont(font);
        lineEdit->setText(method->description);

        ui->verticalLayout_8->addWidget(combo);
        ui->verticalLayout_9->addWidget(lineEdit);

        MethodGui mgui;
        mgui.combo = combo;
        mgui.combo->setEditable(true);
        mgui.edit = lineEdit;
        combos.push_back(mgui);
    }
}

void AddForm::closeWindow()
{
    close();
}

void AddForm::addMethod(const Handbook &m)
{
    QComboBox* combo = new QComboBox(this);
    combo->addItem("", 0);

    foreach (auto method, methods) {
        combo->addItem(method->name, method->id);
    }

    combo->setCurrentIndex(combo->findData(m.id));

    auto lineEdit = new QLineEdit(this);

    auto font = lineEdit->font();
    font.setPointSize(Helper::getFontSize());
    combo->setFont(font);
    lineEdit->setFont(font);

    ui->verticalLayout_8->addWidget(combo);
    ui->verticalLayout_9->addWidget(lineEdit);

    MethodGui mgui;
    mgui.combo = combo;
    mgui.combo->setEditable(true);
    mgui.edit = lineEdit;
    combos.push_back(mgui);
}

void AddForm::setRepairer(int id)
{
    ui->repairer->setCurrentIndex(ui->repairer->findData(id));
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
        combo->addItem(method->name, method->id);
    }

    auto lineEdit = new QLineEdit(this);

    auto font = lineEdit->font();
    font.setPointSize(Helper::getFontSize());
    combo->setFont(font);
    lineEdit->setFont(font);

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

    if ( ui->state->text().trimmed() == "")
    {
        showInfo("Не определено состояние!");
        return;
    }
    if (ui->clientCost->value() != 0 && ui->repairCost->value() != 0)
    {
        showInfo("Должна быть заполнена цена для клиента или цена ремонта!");
        return;
    }

    creatingCard.barCode = ui->barCode->text();
    creatingCard.clientId = ui->client->currentData().toInt();
    creatingCard.complaints = ui->complains->toPlainText();
    creatingCard.id = ui->cardNumber->text().toInt();
    creatingCard.note = ui->note->toPlainText();
    creatingCard.readyDate = QDate::fromString(ui->readyDate->text(), "dd.MM.yyyy");
    creatingCard.reason = ui->reason->toPlainText();
    creatingCard.receiveFromClientDate = QDate::fromString(ui->receiveDate->text(),"dd.MM.yyyy");
    creatingCard.repairerId = ui->repairer->currentData().toInt();
    creatingCard.returnDate = QDate::fromString(ui->returnDate->text(),"dd.MM.yyyy");
    creatingCard.costForClient = ui->clientCost->value();
    creatingCard.costRepair = ui->repairCost->value();
    creatingCard.sendDate = QDate::fromString(ui->sendDate->text(), "dd.MM.yyyy");
    creatingCard.receiveFromFactoryDate = QDate::fromString(ui->receiveDate2->text(), "dd.MM.yyyy");

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

    if (this->mode == Adding)
        addSignal(creatingCard, cardMethods);
    else
        editSignal(creatingCard, cardMethods);
}

void AddForm::on_barCode_textChanged(const QString &arg1)
{
    if (arg1.count() != barCodeLenght)

        return;

    emit barCodeFinish(arg1);

    auto data = Helper::ParseBarcode(arg1);

    ui->createMonth->setText(data.month);
    ui->createYear->setText("20"+data.year);
}

void AddForm::on_pushButton_12_clicked()
{
    this->close();
}

void AddForm::on_sendDate_textChanged(const QString &arg1)
{
    if (arg1.length() < 10)
    {
        ui->receiveDate2->setDisabled(true);
        ui->returnDate->setEnabled(true);
        ui->readyDate->setEnabled(true);
        ui->receiveDate2->clear();
    }
    else
    {
        ui->receiveDate2->setEnabled(true);
        ui->returnDate->setEnabled(false);
        ui->readyDate->setEnabled(false);
        ui->readyDate->clear();
    }

    updateState();
}

void AddForm::showState(int id)
{
    creatingCard.stateId = id;
    for (auto state : states)
        if (state->id == id)
            ui->state->setText(state->name);
}

void AddForm::updateState()
{
    if (ui->receiveDate->text().length() < 10 && ui->returnDate->text().length() < 10)
    {
        ui->state->clear();
        return;
    }

    if (ui->receiveDate->text().length() ==10 && ui->sendDate->text().length() ==10 && ui->receiveDate2->text().length() < 10)
        showState(sendStateId);
    else if (ui->receiveDate->text().length() ==10 && ui->readyDate->text().length() ==10 && ui->returnDate->text().length() < 10)
        showState(readyStateId);
    else if (ui->returnDate->text().length() ==10)
        showState(returnStateId);
    else
         showState(repairStateId);
}

void AddForm::setMode(const FormMode &value)
{
    mode = value;
    if (mode == Adding)
        this->setWindowTitle("Ремонтная карта (добавление)");
    else
        this->setWindowTitle("Ремонтная карта (редактирование)");
}

void AddForm::on_receiveDate_textChanged(const QString &arg1)
{
    if (arg1.length() < 10)
    {
        ui->receiveDate2->setDisabled(true);
        ui->readyDate->setDisabled(true);
        ui->sendDate->setDisabled(true);

        ui->receiveDate2->clear();
        ui->readyDate->clear();
        ui->sendDate->clear();
    }
    else
    {
        ui->receiveDate2->setDisabled(true);
        ui->readyDate->setDisabled(false);
        ui->sendDate->setDisabled(false);
    }

    ui->returnDate->setDisabled(false);
    updateState();
}

void AddForm::on_receiveDate2_textChanged(const QString &arg1)
{
    if (arg1.length() < 10 && ui->sendDate->text().length() == 10)
    {
        ui->readyDate->setEnabled(false);
        ui->returnDate->setEnabled(false);
        ui->readyDate->clear();
    }
    else
    {
            ui->returnDate->setEnabled(true);
            ui->readyDate->setEnabled(true);
    }
    updateState();
}

void AddForm::on_readyDate_textChanged(const QString &arg1)
{
    if (arg1.length() < 10)
    {
        ui->sendDate->setDisabled(false);
        if (ui->sendDate->text().length() == 10)
            ui->receiveDate2->setDisabled(false);
    }
    else
    {
        ui->sendDate->setDisabled(true);
        ui->receiveDate2->setDisabled(true);
    }

    updateState();
}

void AddForm::on_returnDate_textChanged(const QString &arg1)
{
    if (arg1.length() < 10)
    {
        ui->receiveDate->setDisabled(false);
        ui->sendDate->setDisabled(false);
        ui->readyDate->setDisabled(false);
        if (ui->sendDate->text().length() == 10)
            ui->receiveDate2->setDisabled(false);
    }
    else
    {
        ui->receiveDate2->setDisabled(true);
        ui->receiveDate->setDisabled(true);
        ui->readyDate->setDisabled(true);
        ui->sendDate->setDisabled(true);
        ui->receiveDate2->setDisabled(true);
    }

    updateState();
}

void AddForm::on_pushButton_3_clicked()
{
    emit editRepairers();
}

void AddForm::on_pushButton_6_clicked()
{
    emit editMethods();
}

void AddForm::on_pushButton_clicked()
{
    emit editClients();
}

void AddForm::on_pushButton_2_clicked()
{
    emit editProducts();
}
