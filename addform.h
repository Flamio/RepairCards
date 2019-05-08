#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QHBoxLayout>
#include "iaddview.h"
#include "cardmethod.h"
#include <QEvent>
#include "product.h"

namespace Ui {
class AddForm;
}

struct MethodGui
{
    QComboBox* combo;
    QLineEdit* edit;
};

class DateOnDoubleClick : public QObject {
    bool eventFilter(QObject *watched, QEvent *event) {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if (((QWidget*)watched)->isEnabled())
                QMetaObject::invokeMethod(watched, "setText", Q_ARG(QString, QDate::currentDate().toString("dd.MM.yyyy")));
        }

        return QObject::eventFilter(watched, event);
    }
public:
    explicit DateOnDoubleClick(QObject * parent = nullptr) : QObject(parent) {
        addTo(parent);
    }
    void addTo(QObject * obj) {
        if (obj) obj->installEventFilter(this);
    }
};


class AddForm : public QMainWindow, public IAddView
{
    Q_OBJECT

public:
    explicit AddForm(QWidget *parent = 0);
    ~AddForm();

    void setMethods(QVector<Handbook*>&);
    virtual void setStates(QVector<Handbook*>&);
    virtual void setRepairers(QVector<Handbook*>&);
    virtual void setClients(QVector<Handbook*>&);
    virtual void showInfo(QString);
    virtual void setProduct(const Product& product);
    virtual void showWindow();
    virtual void setCard(const RepairCard &card, QVector<CardMethod>* methods=nullptr);
    virtual void closeWindow();
    virtual void addMethod(const Handbook&);
    virtual void setRepairer(int id);
    virtual void barCodeFinishEmit();
    virtual void setClient(int id);

    void setMode(const FormMode &value);

    FormMode getMode();

signals:
    void barCodeFinish(QString barcode, bool isOwen);
    void addSignal(const RepairCard& card, const QVector<CardMethod>& methods);
    void editSignal(const RepairCard& card, const QVector<CardMethod>& methods);
    void editRepairers();
    void editMethods();
    void editClients();
    void editProducts();
    void showProdictSearch();

private slots:

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_barCode_textChanged(const QString &arg1);

    void on_pushButton_12_clicked();

    void on_sendDate_textChanged(const QString &arg1);

    void on_receiveDate_textChanged(const QString &arg1);

    void on_receiveDate2_textChanged(const QString &arg1);

    void on_readyDate_textChanged(const QString &arg1);

    void on_returnDate_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_client_activated(int index);

    void on_checkBox_clicked(bool checked);

    void on_selectProductButton_clicked();

private:
    const int sendStateId = 4;
    const int readyStateId = 1;
    const int returnStateId = 2;
    const int repairStateId = 3;
    const int barCodeLenght = 17;

    Ui::AddForm *ui;
    QVector<Handbook*> clients;
    QVector<MethodGui> combos;
    QVector<Handbook*> methods;
    QVector<Handbook*> states;
    RepairCard creatingCard;
    void addGuiMethodsItem();

    DateOnDoubleClick* dateOnReturnDate = nullptr;
    DateOnDoubleClick* dateOnSendDate = nullptr;
    DateOnDoubleClick* dateOnReadyDate = nullptr;
    DateOnDoubleClick* dateOnReceive2Date = nullptr;
    DateOnDoubleClick* dateOnReceiveDate = nullptr;

    FormMode mode = Adding;

    void showState(int id);
    void updateState();

    Client* getClientById(int id);
    QRegExpValidator* barcodeValidatorOwen;
    QRegExpValidator* barcodeValidator;
};

#endif // MAINWINDOW_H
