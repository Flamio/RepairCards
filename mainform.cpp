#include "mainform.h"
#include "ui_mainform.h"
#include "helper.h"
#include <QMessageBox>

MainForm::MainForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    ui->current_card_index->setValidator(new QIntValidator());
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::setCard(const RepairCard &card, const QVector<CardMethod> &methods)
{
    if (card.returnDate.toString("dd.MM.yyyy") != "")
        this->setStyleSheet("QLabel, QLineEdit, QTableWidget, QPlainTextEdit {color:gray} "
                            " QLineEdit#current_card_index {color:black} QLineEdit#cards_count {color:black}");
    else
        this->setStyleSheet("QLabel, QLineEdit, QTableWidget, QPlainTextEdit {color:black}");

    ui->barcode->setText(card.barCode);
    if (card.product.isOwen)
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
    ui->client->setText(QString("%1; %2; %3").arg(card.client.name).arg(card.client.phone).arg(card.client.person));
    ui->clientCost->setText(QString::number(card.costForClient));
    ui->complains->setPlainText(card.complaints);
    ui->id->setText(QString::number(card.id));
    ui->cards_count->setText(QString("%1").arg(card.allIndexes));
    ui->current_card_index->setText(QString("%1").arg(card.currentIndex));
    ui->note->setPlainText(card.note);
    ui->product->setText(card.product.name);
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

    dialog = new MainForm(this);
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

void MainForm::on_showSendedProducts_clicked()
{
    emit showSendedProducts();
}

void MainForm::on_pushButton_2_clicked()
{
    emit showExtremeCard(ExtremeCardType::Last);
}

void MainForm::on_pushButton_3_clicked()
{
    emit showExtremeCard(ExtremeCardType::First);
}

void MainForm::on_current_card_index_editingFinished()
{
    emit showCardByIndex(ui->current_card_index->text().toInt());
}

void MainForm::on_pushButton_4_clicked()
{
    callbacks.searchCards();
}
