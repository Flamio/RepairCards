#include "mainform.h"
#include "ui_mainform.h"
#include "helper.h"

MainForm::MainForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::setCard(const RepairCard &card, const QVector<CardMethod> &methods)
{
    ui->barcode->setText(card.barCode);
    auto data = Helper::ParseBarcode(card.barCode);
    ui->createMonth->setText(data.month);
    ui->createYear->setText("20"+data.year);
    ui->client->setText(QString("%1 %2 %3 %4").arg(card.client.name).arg(card.client.phone).arg(card.client.contact).arg(card.client.address));
    ui->clientCost->setText(QString::number(card.costForClient));
    ui->complains->setPlainText(card.complaints);
    ui->id->setText(QString::number(card.id));
    ui->navigation->setText(QString("%1/%2").arg(card.currentIndex).arg(card.allIndexes));
    ui->note->setPlainText(card.note);
    ui->product->setText(card.productName);
    ui->ready->setText(card.readyDate.toString("dd.MM.yyyy"));
    ui->returnDate->setText(card.returnDate.toString("dd.MM.yyyy"));
    ui->receive->setText(card.receiveDate.toString("dd.MM.yyyy"));
    ui->state->setText(card.state);
    ui->reason->setPlainText(card.reason);
    ui->repairCost->setText(QString::number(card.costRepair));
    ui->repairer->setText(card.repairer);

    for (auto method : this->methods)
    {
        delete method.name;
        delete method.description;
    }

    this->methods.clear();

    for (auto method : methods)
    {
        auto name = new QLineEdit();
        auto description = new QLineEdit();
        UiMethod m;
        m.name = name;
        m.description = description;
        this->methods.push_back(m);
        name->setText(method.methodName);
        name->setReadOnly(true);
        description->setReadOnly(true);
        description->setText(method.description);
        ui->verticalLayout_2->addWidget(name);
        ui->verticalLayout_3->addWidget(description);
    }
}

void MainForm::on_pushButton_7_clicked()
{
    emit navigation(false);
}

void MainForm::on_pushButton_8_clicked()
{
    emit navigation(true);
}

void MainForm::on_pushButton_10_clicked()
{
    emit add();
}
