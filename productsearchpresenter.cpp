#include "productsearchpresenter.h"

ProductSearchPresenter::ProductSearchPresenter(bool isNotOwenOnly, QObject *parent) : QObject(parent)
{
    this->isNotOwenOnly = isNotOwenOnly;
}

void ProductSearchPresenter::setView(IProductSearchView *value)
{
    view = value;
    connect(dynamic_cast<QObject*>(view), SIGNAL(searchProduct(const QString&)), this, SLOT(onSearchProduct(const QString&)));
    connect(dynamic_cast<QObject*>(view), SIGNAL(done(Product)), this, SLOT(onDone(Product)));
}

void ProductSearchPresenter::showView()
{
    this->view->showWindow();
}

void ProductSearchPresenter::closeView()
{
    this->view->closeWindow();
}

void ProductSearchPresenter::onSearchProduct(const QString &name)
{
    auto db = DatabaseConnector::getInstance();
    auto products = db->getProductsByName(name, isNotOwenOnly);
    view->setProducts(products);
}

void ProductSearchPresenter::onDone(Product p)
{
    product = p;
    emit done();
}

Product* ProductSearchPresenter::getProduct()
{
    return &product;
}
