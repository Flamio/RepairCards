#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->comboBox_2->addItem("", -1);

    foreach (auto state, states)
        ui->comboBox_2->addItem(state.name, state.id);
}

void MainWindow::setRepairers(QVector<Handbook> &repairers)
{
    ui->comboBox_3->addItem("", -1);

    foreach (auto state, repairers)
        ui->comboBox_3->addItem(state.name, state.id);
}

void MainWindow::setClients(QHash<int,Client> &clients)
{
    ui->comboBox->addItem("", -1);

    QVector<Client> vector;

    foreach (auto client, clients)
        vector.push_back(client);

    qSort(vector.begin(), vector.end(), [] (const Client& one, const Client& two) { return one.name < two.name;});

    foreach (auto client, vector)
        ui->comboBox->addItem(client.name, client.id);

    this->clients = clients;
}

void MainWindow::setMode(MainViewMode mode)
{
    if (mode == MainViewMode::Reading)
    {
        ui->frame_2->setVisible(true);
        ui->frame->setVisible(false);
    }
    else
    {
        ui->frame_2->setVisible(false);
        ui->frame->setVisible(true);
    }
}

void MainWindow::on_comboBox_activated(int index)
{
    auto hashIndex  = ui->comboBox->itemData(index);
    auto address = clients[hashIndex.toInt()].address;
    ui->lineEdit->setText(address);
}

void MainWindow::on_pushButton_4_clicked()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QComboBox* combo = new QComboBox(this);
    combo->setMinimumWidth(510);
    combo->addItem("", -1);

    foreach (auto method, methods) {
       combo->addItem(method.name, method.id);
    }

    auto lineEdit = new QLineEdit(this);
    layout->addWidget(combo);
    layout->addWidget(lineEdit);
    ui->verticalLayout_3->addLayout(layout);

    MethodGui mgui;
    mgui.combo = combo;
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
    setMode(Adding);
}

void MainWindow::on_pushButton_11_clicked()
{
}
