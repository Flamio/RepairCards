#include "analyzeform.h"
#include "ui_analyzeform.h"
#include <QMessageBox>
#include <QFileDialog>

AnalyzeForm::AnalyzeForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalyzeForm)
{
    ui->setupUi(this);
    analyze.loadWeights();
}

AnalyzeForm::~AnalyzeForm()
{
    delete ui;
}

void AnalyzeForm::on_pushButton_clicked()
{
    std::vector<float> ins;

    ins.push_back(ui->lineEdit->text().toFloat());
    ins.push_back(ui->lineEdit_2->text().toFloat());
    ins.push_back(ui->lineEdit_3->text().toFloat());
    ins.push_back(ui->checkBox->checkState() ? 1 : 0);

    float result = analyze.calcResult(ins);

    QMessageBox msg;
    msg.setText(QString("Оценка изделия - %1").arg(result*10.0f));
    msg.exec();
}

void AnalyzeForm::on_pushButton_2_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.csv");
    analyze.train(0.001, str);
    analyze.saveWeights();
}
