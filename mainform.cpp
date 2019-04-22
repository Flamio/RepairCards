#include "mainform.h"
#include "ui_mainform.h"
#include "helper.h"
#include <QMessageBox>

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
    if (card.isOwen)
    {
        auto data = Helper::ParseBarcode(card.barCode);
        ui->createMonth->setText(data.month);
        ui->createYear->setText("20"+data.year);
    }
    else
    {
        ui->createMonth->setText(card.month);
        ui->createYear->setText(card.year);
    }
    ui->client->setText(QString("%1 %2 %3 %4").arg(card.client.name).arg(card.client.phone).arg(card.client.person).arg(card.client.address));
    ui->clientCost->setText(QString::number(card.costForClient));
    ui->complains->setPlainText(card.complaints);
    ui->id->setText(QString::number(card.id));
    ui->navigation->setText(QString("%1/%2").arg(card.currentIndex).arg(card.allIndexes));
    ui->note->setPlainText(card.note);
    ui->product->setText(card.productName);
    ui->ready->setText(card.readyDate.toString("dd.MM.yyyy"));
    ui->returnDate->setText(card.returnDate.toString("dd.MM.yyyy"));
    ui->receive->setText(card.receiveFromClientDate.toString("dd.MM.yyyy"));
    ui->state->setText(card.state);
    ui->reason->setPlainText(card.reason);
    ui->repairCost->setText(QString::number(card.costRepair));
    ui->repairer->setText(card.repairer);
    ui->receive2->setText(card.receiveFromFactoryDate.toString("dd.MM.yyyy"));
    ui->send->setText(card.sendDate.toString("dd.MM.yyyy"));

    ui->tableWidget->setRowCount(0);

    int i = 0;

    for (auto method : methods)
    {
        auto item =  new QTableWidgetItem(method.methodName);
        item->setFlags(Qt::ItemIsEnabled);
        auto item2 =  new QTableWidgetItem(method.description);
        item2->setFlags(Qt::ItemIsEnabled);

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0, item);
        ui->tableWidget->setItem(i,1, item2);
        ui->tableWidget->setRowHeight(i,20);
        i++;
    }
}

IMainView *MainForm::newDialog()
{
    if (dialog != nullptr)
        delete dialog;

    dialog = new MainForm();
    dialog->setIsDialog(true);
    dialog->hideNavigationPanel();
    return dialog;
}

void MainForm::closeEvent(QCloseEvent *event)
{
    if (isDialog)
    {
        event->accept();
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Выход", "Вы действительно хотите закрыть программу?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
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

void MainForm::on_pushButton_9_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление", "Вы действительно хотите удалить ремонтную карту?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
        deleteSignal(ui->id->text().toInt());
}

void MainForm::on_pushButton_clicked()
{
    emit edit(ui->id->text().toInt());
}

void MainForm::setIsDialog(bool value)
{
    isDialog = value;
}

void MainForm::hideNavigationPanel()
{
    ui->frame_2->setVisible(false);
}

void MainForm::on_pushButton_6_clicked()
{
    auto type = (PrintType::PrintType)ui->print->currentIndex();
    auto cardId = ui->id->text().toInt();
    emit print(cardId,type);
}
