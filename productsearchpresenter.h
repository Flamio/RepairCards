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

    void setDb(DatabaseConnector *value);

    IProductSearchView *getView() const;

    Handbook* getProduct();

signals:
    void done();

public slots:

private slots:
    void onSearchProduct(const QString& number);
    void onDone();

private:
    IProductSearchView* view = nullptr;
    DatabaseConnector* db = nullptr;
    Handbook product;
};

#endif // PRODUCTSEARCHPRESENTER_H
