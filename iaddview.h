#ifndef IADDVIEW_H
#define IADDVIEW_H
#include <QVector>

#include "handbook.h"
#include "client.h"
#include "addformmode.h"
#include "repaircard.h"
#include "cardmethod.h"

class IAddView
{
public:
    virtual void setMethods(QVector<Handbook>&) = 0;
    virtual void setStates(QVector<Handbook>&) = 0;
    virtual void setRepairers(QVector<Handbook>&) = 0;
    virtual void setClients(QHash<int,Client>&) = 0;    
    virtual void showInfo(QString) = 0;
    virtual void barCodeFinish(QString barcode) = 0;
    virtual void addSignal(const RepairCard& card, const QVector<CardMethod>& methods) = 0;
    virtual void editSignal(const RepairCard& card, const QVector<CardMethod>& methods) = 0;
    virtual void setProduct(const Handbook& product) = 0;
    virtual void showWindow() = 0;
    virtual void setCard(const RepairCard &card, QVector<CardMethod>* methods=nullptr) = 0;
    virtual void closeWindow() = 0;
    virtual void setMode(const AddFormMode &value) = 0;

signals:
};

Q_DECLARE_INTERFACE(IAddView, "IAddView")

#endif // IADDVIEW_H
