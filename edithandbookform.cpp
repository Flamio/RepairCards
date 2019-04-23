#include "edithandbookform.h"
#include "ui_edithandbookform.h"
#include <QMessageBox>

EditHandbookForm::EditHandbookForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditHandbookForm)
{
    ui->setupUi(this);
    setMode(Editing);
}

EditHandbookForm::~EditHandbookForm()
{
    delete ui;
}

void EditHandbookForm::setHandbooks(const QVector<Handbook*> &handbooks)
{

    for (auto h : this->handbooks)
        delete h;

    this->handbooks.clear();


    this->handbooks = handbooks;
    ui->handbook->clear();
    for (auto h : handbooks)
        ui->handbook->addItem(h->name, h->id);
}

void EditHandbookForm::showWindow()
{
    this->show();
}

void EditHandbookForm::closeWindow()
{
    this->close();
}

void EditHandbookForm::on_pushButton_3_clicked()
{
    if (mode == Editing)
        close();
    else
        setMode(Editing);
}

void EditHandbookForm::on_handbook_currentIndexChanged(int index)
{
    if (handbooks.count() == 0)
        return;

    if (index < 0 || handbooks.count() < index + 1)
        index = 0;

    auto hb = handbooks[index];
    ui->id->setText(QString::number(hb->id));
    ui->name->setText(hb->name);
}

void EditHandbookForm::on_pushButton_2_clicked()
{
    if (ui->name->text() == "")
    {
        QMessageBox msg;
        msg.setText("Пустое название!");
        msg.exec();
        return;
    }

    if (handbook != nullptr)
        delete handbook;

    fillHandbookFields(&handbook);

    if (mode == Adding)
        emit add(handbook);
    else
        emit edit(handbook);
}

void EditHandbookForm::setMode(FormMode mode)
{
    this->mode = mode;
    if (mode == Adding)
    {
        ui->handbook->setVisible(false);
        ui->pushButton->setVisible(false);
        ui->id->setVisible(false);
        ui->idLabel->setVisible(false);
        ui->del->setVisible(false);
        ui->pushButton_2->setText("ОК");
        clearFieldsOnAdd();
    }
    else
    {
        ui->del->setVisible(true);
        ui->handbook->setVisible(true);
        ui->pushButton->setVisible(true);
        ui->id->setVisible(true);
        ui->idLabel->setVisible(true);
        ui->pushButton_2->setText("Редактировать");
        if (handbooks.count() > 0)
            fillFieldsOnEdit(ui->handbook->currentIndex());
    }
}

void EditHandbookForm::setHandbook(int id)
{
    ui->handbook->setCurrentIndex(ui->handbook->findData(id));
}

void EditHandbookForm::on_pushButton_clicked()
{
    setMode(Adding);
}

void EditHandbookForm::on_del_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление", QString("Вы действительно хотите удалить \"%1\"?").arg(ui->name->text()),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        emit deleteHandbook(ui->id->text().toInt());
        ui->id->clear();
        clearFieldsOnAdd();
    }
}

Ui::EditHandbookForm *EditHandbookForm::getUi() const
{
    return ui;
}

QVector<Handbook*>* EditHandbookForm::getHandbooks()
{
    return &handbooks;
}

Handbook *EditHandbookForm::getHandbook() const
{
    return handbook;
}

void EditHandbookForm::fillHandbookFields(Handbook** hb)
{

    auto h = new Handbook();

    h->id = ui->id->text().toInt();
    h->name = ui->name->text();

    *hb = h;
}

void EditHandbookForm::clearFieldsOnAdd()
{
    ui->name->clear();
}

void EditHandbookForm::fillFieldsOnEdit(int currentIndex)
{
    ui->name->setText(handbooks[currentIndex]->name);
}
