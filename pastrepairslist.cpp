#include "pastrepairslist.h"
#include "ui_pastrepairslist.h"

PastRepairsList::PastRepairsList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PastRepairsList)
{
    ui->setupUi(this);
}

PastRepairsList::~PastRepairsList()
{
    delete ui;
}

void PastRepairsList::showWindow()
{
    show();
}

void PastRepairsList::closeWindow()
{
    close();
}

void PastRepairsList::setCards(QVector<RepairCard> &cards)
{
    this->cards = cards;

    for (auto card : cards)
        ui->listWidget->addItem(card.receiveFromClientDate.toString("dd.MM.yyyy"));
}

void PastRepairsList::on_pushButton_clicked()
{
    close();
}

void PastRepairsList::on_pushButton_2_clicked()
{
    auto index = ui->listWidget->currentIndex().row();
    emit cardClicked(this->cards.at(index).id);
}

void PastRepairsList::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto index = ui->listWidget->row(item);
    emit cardClicked(this->cards.at(index).id);
}
