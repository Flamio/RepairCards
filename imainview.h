#ifndef IMAINVIEW_H
#define IMAINVIEW_H
#include "repaircard.h"
#include "cardmethod.h"
#include "iview.h"
#include "printtype.h"

class IMainView : public IView
{
public:
    virtual void add() = 0;
    virtual void edit(int id) = 0;
    virtual void showSendedProducts() = 0;
    virtual void setCard(const RepairCard &card, const QVector<CardMethod>& methods) = 0;
    virtual void navigation(bool) = 0;
    virtual void deleteSignal(int id) = 0;
    virtual IMainView* newDialog() = 0;
signals:
    virtual void print(int ,PrintType::PrintType&) = 0;
};

Q_DECLARE_INTERFACE(IMainView, "IMainView")

#endif // IMAINVIEW_H
