#ifndef IADDVIEW_H
#define IADDVIEW_H
#include <QVector>

#include "handbook.h"
#include "client.h"
#include "formmode.h"
#include "repaircard.h"
#include "cardmethod.h"
#include "iview.h"
#include <QCompleter>

class IAddView : public IView
{
public:
    virtual void setMethods(QVector<Handbook*>&) = 0;
    virtual void setStates(QVector<Handbook*>&) = 0;
    virtual void setRepairers(QVector<Handbook*>&) = 0;
    virtual void setClients(QVector<Handbook*>&) = 0;
    virtual void showInfo(QString) = 0;
    virtual void barCodeFinish(QString barcode) = 0;
    virtual void addSignal(const RepairCard& card, const QVector<CardMethod>& methods) = 0;
    virtual void editSignal(const RepairCard& card, const QVector<CardMethod>& methods) = 0;
    virtual void setProduct(const Handbook& product) = 0;
    virtual void setCard(const RepairCard &card, QVector<CardMethod>* methods=nullptr) = 0;
    virtual void setMode(const FormMode &value) = 0;
    virtual void setRepairer(int id) = 0;
    virtual void setClient(int id) = 0;
    virtual void editRepairers() = 0;
    virtual void editMethods() = 0;
    virtual void addMethod(const Handbook&) = 0;
    virtual void barCodeFinishEmit() = 0;
    virtual void setProductCompleter(QCompleter*) = 0;

signals:
    virtual void editClients() = 0;
    virtual void editProducts() = 0;
    virtual void checkProduct(int) = 0;
};

Q_DECLARE_INTERFACE(IAddView, "IAddView")

#endif // IADDVIEW_H
