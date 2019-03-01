#ifndef IMAINVIEW_H
#define IMAINVIEW_H
#include <QVector>

#include "handbook.h"
#include "client.h"
#include "mainviewmode.h"
#include "repaircard.h"
#include "cardmethod.h"

class IMainView
{
public:
    virtual void setMethods(QVector<Handbook>&) = 0;
    virtual void setStates(QVector<Handbook>&) = 0;
    virtual void setRepairers(QVector<Handbook>&) = 0;
    virtual void setClients(QHash<int,Client>&) = 0;
    virtual void setMode(MainViewMode) = 0;
    virtual void showInfo(QString) = 0;
    virtual void setCard(const RepairCard&, const QVector<CardMethod>&) = 0;
    virtual void barCodeFinish(QString barcode) = 0;
    virtual void setProduct(const Handbook& product) = 0;
    virtual void addSignal(const RepairCard& card, const QVector<CardMethod>& methods) = 0;
    virtual void newCard() = 0;
    virtual void cancelAdding() = 0;
    virtual void navigation(bool forward) = 0;

signals:
};

Q_DECLARE_INTERFACE(IMainView, "IMainView")

#endif // IMAINVIEW_H
