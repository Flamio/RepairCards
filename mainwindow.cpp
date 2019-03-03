#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMode(Reading);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMethods(QVector<Handbook>& methods)
{
    this->methods = methods;
}

void MainWindow::setStates(QVector<Handbook> &states)
{
    ui->state->addItem("", 0);

    foreach (auto state, states)
        ui->state->addItem(state.name, state.id);
}

void MainWindow::setRepairers(QVector<Handbook> &repairers)
{
    ui->repairer->addItem("", 0);

    foreach (auto state, repairers)
        ui->repairer->addItem(state.name, state.id);
}

void MainWindow::setClients(QHash<int,Client> &clients)
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

void MainWindow::setMode(MainViewMode mode)
{
    this->mode = mode;
    if (mode == MainViewMode::Reading)
    {
        ui->frame_2->setVisible(true);
        ui->frame->setVisible(false);
        ui->pushButton_4->setVisible(false);
        ui->pushButton_5->setVisible(false);

        ui->barCode->setReadOnly(true);
        ui->complains->setReadOnly(true);
        ui->note->setReadOnly(true);
        ui->reason->setReadOnly(true);
        ui->receiveDate->setReadOnly(true);
        ui->returnDate->setReadOnly(true);
        ui->readyDate->setReadOnly(true);
        ui->repairCost->setReadOnly(true);
        ui->clientCost->setReadOnly(true);

    }
    else
    {
        ui->frame_2->setVisible(false);
        ui->frame->setVisible(true);
        ui->pushButton_4->setVisible(true);
        ui->pushButton_5->setVisible(true);

        ui->barCode->setReadOnly(false);
        ui->complains->setReadOnly(false);
        ui->note->setReadOnly(false);
        ui->reason->setReadOnly(false);
        ui->receiveDate->setReadOnly(false);
        ui->returnDate->setReadOnly(false);
        ui->readyDate->setReadOnly(false);
        ui->repairCost->setReadOnly(false);
        ui->clientCost->setReadOnly(false);
    }
}

void MainWindow::showInfo(QString info)
{
    QMessageBox msg;
    msg.setText(info);
    msg.exec();
}

void MainWindow::setCard(const RepairCard &card, const QVector<CardMethod>& methods)
{    
    ui->cardNumber->setText(QString::number(card.id));
    ui->barCode->setText(card.barCode);
    ui->client->setCurrentIndex(ui->client->findData(card.clientId));
    auto hashIndex  = ui->client->currentData();
    auto address = clients[hashIndex.toInt()].address;
    ui->clientAddress->setText(address);

    ui->complains->setText(card.complaints);
    ui->note->setText(card.note);
    ui->reason->setText(card.reason);
    ui->product->setText(card.productName);
    ui->receiveDate->setDate(card.receiveDate);
    ui->returnDate->setDate(card.returnDate);
    ui->repairer->setCurrentIndex(ui->repairer->findData(card.repairerId));
    ui->state->setCurrentIndex(ui->state->findData(card.stateId));
    ui->readyDate->setDate(card.readyDate);
    ui->repairCost->setValue(card.costRepair);
    ui->clientCost->setValue(card.costForClient);
    ui->navigation->setText(QString("%1/%2").arg(card.currentIndex).arg(card.allIndexes));

    foreach (MethodGui item, combos) {
        delete item.combo;
        delete item.edit;
        delete item.layout;
        combos.pop_back();
    }

    foreach (CardMethod m, methods) {
        QHBoxLayout* layout = new QHBoxLayout(this);
        QComboBox* combo = new QComboBox(this);
        combo->setMinimumWidth(510);
        combo->addItem("", 0);

        foreach (auto method, this->methods) {
           combo->addItem(method.name, method.id);
        }

        combo->setCurrentIndex(combo->findData(m.methodId));

        auto lineEdit = new QLineEdit(this);
        lineEdit->setText(m.description);
        layout->addWidget(combo);
        layout->addWidget(lineEdit);
        ui->verticalLayout_3->addLayout(layout);

        MethodGui mgui;
        mgui.combo = combo;
        mgui.combo->setEditable(true);
        mgui.edit = lineEdit;
        mgui.layout = layout;
        combos.push_back(mgui);
    }
}

void MainWindow::setProduct(const Handbook& product)
{
    ui->product->setText(product.name);
    creatingCard.productId = product.id;
}

void MainWindow::on_client_activated(int index)
{
    auto hashIndex  = ui->client->itemData(index);
    auto address = clients[hashIndex.toInt()].address;
    ui->clientAddress->setText(address);
}

void MainWindow::on_pushButton_4_clicked()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QComboBox* combo = new QComboBox(this);
    combo->setMinimumWidth(510);
    combo->addItem("", 0);

    foreach (auto method, methods) {
       combo->addItem(method.name, method.id);
    }

    auto lineEdit = new QLineEdit(this);
    layout->addWidget(combo);
    layout->addWidget(lineEdit);
    ui->verticalLayout_3->addLayout(layout);

    MethodGui mgui;
    mgui.combo = combo;
    mgui.combo->setEditable(true);
    mgui.edit = lineEdit;
    mgui.layout = layout;
    combos.push_back(mgui);
}

void MainWindow::on_pushButton_5_clicked()
{
    if (combos.count() == 0)
        return;

    MethodGui item = combos.last();
    delete item.combo;
    delete item.edit;
    delete item.layout;
    combos.pop_back();
}

void MainWindow::on_pushButton_10_clicked()
{
    newCard();
    ui->createMonth->clear();
    ui->createYear->clear();
}

void MainWindow::on_pushButton_11_clicked()
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
    creatingCard.receiveDate = ui->receiveDate->date();
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

void MainWindow::on_barCode_textChanged(const QString &arg1)
{
    if (arg1.count() != barCodeLenght)
        return;

    if (mode == Adding)
        barCodeFinish(arg1);

    auto year = arg1.mid(5,2);
    auto month = arg1.mid(7,2);

    ui->createMonth->setText(month);
    ui->createYear->setText("20"+year);
}

void MainWindow::on_pushButton_12_clicked()
{
    cancelAdding();
}

void MainWindow::on_pushButton_8_clicked()
{
    navigation(true);
}

void MainWindow::on_pushButton_7_clicked()
{
    navigation(false);
}
