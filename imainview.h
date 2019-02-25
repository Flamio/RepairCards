#ifndef IMAINVIEW_H
#define IMAINVIEW_H
#include <QVector>

#include "handbook.h"

class IMainView
{
public:
    virtual void setMethods(QVector<Handbook>&) = 0;
    virtual void setStates(QVector<Handbook>&) = 0;
    virtual void setRepairers(QVector<Handbook>&) = 0;
};

#endif // IMAINVIEW_H
