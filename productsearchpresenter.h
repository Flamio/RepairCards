#ifndef PRODUCTSEARCHPRESENTER_H
#define PRODUCTSEARCHPRESENTER_H

#include <QObject>
#include "iproductsearchview.h"
#include "databaseconnector.h"

class ProductSearchPresenter : public QObject
{
    Q_OBJECT
public:
    explicit ProductSearchPresenter(QObject *parent = 0);

    void setView(IProductSearchView *value);

    void showView();
    void closeView();

    Product* getProduct();

signals:
    void done();

public slots:

private slots:
    void onSearchProduct(const QString& name);
    void onDone(Product);

private:
    IProductSearchView* view = nullptr;
    Product product;
};

#endif // PRODUCTSEARCHPRESENTER_H
