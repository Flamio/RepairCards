#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMethods(QVector<Handbook>& methods)
{
    ui->comboBox_4->addItem("", -1);
    ui->comboBox_6->addItem("", -1);
    ui->comboBox_7->addItem("", -1);
    ui->comboBox_8->addItem("", -1);
    ui->comboBox_9->addItem("", -1);

    foreach (auto method, methods) {
        ui->comboBox_4->addItem(method.name, method.id);
        ui->comboBox_6->addItem(method.name, method.id);
        ui->comboBox_7->addItem(method.name, method.id);
        ui->comboBox_8->addItem(method.name, method.id);
        ui->comboBox_9->addItem(method.name, method.id);
    }
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
