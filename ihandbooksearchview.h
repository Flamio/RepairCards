#pragma once

#include "iview.h"
#include <QString>
#include <QVector>
#include "product.h"
#include <functional>

struct HandbookSearchCallbacks
{
    std::function<void(const QString&)> searchHandbook = nullptr;
    std::function<void(int)> done;
};

class IHandbookSearchView : public IView
{
public:
    virtual void setName(const QString& name) = 0;
    virtual void setHandbooks(QVector<Handbook>&) = 0;
    virtual void setCallbacks(const HandbookSearchCallbacks& ) = 0;
};
