#ifndef IMAINVIEW_H
#define IMAINVIEW_H
#include <QVector>

#include "handbook.h"
#include "client.h"
#include "mainviewmode.h"

class IMainView
{
public:
    virtual void setMethods(QVector<Handbook>&) = 0;
    virtual void setStates(QVector<Handbook>&) = 0;
    virtual void setRepairers(QVector<Handbook>&) = 0;
    virtual void setClients(QHash<int,Client>&) = 0;
    virtual void setMode(MainViewMode) = 0;
};

#endif // IMAINVIEW_H
