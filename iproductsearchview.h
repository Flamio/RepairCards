#ifndef IPRODUCTSEARCHVIEW_H
#define IPRODUCTSEARCHVIEW_H

#include "iview.h"
#include <QString>

class IProductSearchView : public IView
{
public:
    virtual void setName(const QString& name) = 0;

signals:
    virtual void searchProduct(const QString& number) = 0;
    virtual void done() = 0;
};


Q_DECLARE_INTERFACE(IProductSearchView, "IProductSearchView")

#endif // IPRODUCTSEARCHVIEW_H
