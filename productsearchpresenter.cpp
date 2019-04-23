#include "productsearchpresenter.h"

ProductSearchPresenter::ProductSearchPresenter(QObject *parent) : QObject(parent)
{

}

void ProductSearchPresenter::setView(IProductSearchView *value)
{
    view = value;
    connect(dynamic_cast<QObject*>(view), SIGNAL(searchProduct(const QString&)), this, SLOT(onSearchProduct(const QString&)));
    connect(dynamic_cast<QObject*>(view), SIGNAL(done()), this, SLOT(onDone()));
}

void ProductSearchPresenter::onSearchProduct(const QString &number)
{
    product = db->getProductByCode(number);
    if (product.id == 0)
    {
        view->setName("");
        return;
    }
    view->setName(product.name);
}

void ProductSearchPresenter::onDone()
{
    onSearchProduct(product.code);
    emit done();
}

Product* ProductSearchPresenter::getProduct()
{
    return &product;
}

IProductSearchView *ProductSearchPresenter::getView() const
{
    return view;
}

void ProductSearchPresenter::setDb(DatabaseConnector *value)
{
    db = value;
}
