#ifndef IPASTREPAIRLIST_H
#define IPASTREPAIRLIST_H
#include "iview.h"
#include "repaircard.h"

class IPastRepairList : public IView
{
public:
    virtual void setCards(QVector<RepairCard>&) = 0;
};

#endif // IPASTREPAIRLIST_H
