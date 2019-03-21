#ifndef IHANDBOOKEDITVIEW_H
#define IHANDBOOKEDITVIEW_H

#include "iview.h"
#include "handbook.h"
#include "formmode.h"

class IHandbookEditView : public IView
{
public:
    virtual void setHandbooks(const QVector<Handbook>&) = 0;
    virtual void setMode(FormMode) = 0;
    virtual void setHandbook(int id) = 0;

signals:
    virtual void add(Handbook&) = 0;
    virtual void edit(const Handbook&) = 0;
    virtual void deleteHandbook(int id) =0;
};

#endif // IHANDBOOKEDITVIEW_H
