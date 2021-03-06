#ifndef ADDPRESENTER_H
#define ADDPRESENTER_H

#include <QObject>
#include "iaddview.h"
#include "databaseconnector.h"
#include "cardmethod.h"
#include "ihandbookeditview.h"
#include "ipastrepairlist.h"
#include "ihandbooksearchview.h"

class AddPresenter : public QObject
{
    Q_OBJECT
public:
    explicit AddPresenter(QObject *parent = 0);

    void setAddView(IAddView *value);
    void setDatabaseConnector(const DatabaseConnector &value);
    void start();    

    IAddView *getAddView() const;

    void setRepairerEditView(IHandbookEditView *value);

    void setMethodEditView(IHandbookEditView *value);

    void setClientEditView(IHandbookEditView *value);

    void setProductEditView(IHandbookEditView *value);

    void setPastPrepareList(IPastRepairList *value);

    void setProductSearch(IHandbookSearchView *value);

signals:
    void addComplete();
    void editComplete(int id);

public slots:

private slots:
    void onBarCodeFinish(QString, bool);
    void onAdd(const RepairCard&, const QVector<CardMethod>&);
    void onEdit(const RepairCard&, const QVector<CardMethod>&);
    void onEditRepairers();
    void onEditMethods();
    void onMethodAdd(Handbook*);
    void onMethodEdit(Handbook*);
    void onDeleteMethod(int id);
    void onEditClients();
    void onEditProducts();

    void onRepairerAdd(Handbook*);
    void onRepairerEdit(Handbook*);
    void onDeleteRepairer(int id);

    void onProductAdd(Handbook*);
    void onProductEdit(Handbook*);
    void onDeleteProduct(int id);

    void onClientAdd(Handbook*);
    void onClientEdit(Handbook*);
    void onDeleteClient(int id);
    void checkPastRepairs(int productId, const QString& barcode);
    void onShowProdictSearch();

private:
    IAddView* addView = nullptr;
    IHandbookEditView* repairerEditView = nullptr;
    IHandbookEditView* methodEditView = nullptr;
    IHandbookEditView* clientEditView = nullptr;
    IHandbookEditView* productEditView = nullptr;
    IHandbookSearchView* productSearch = nullptr;
    IPastRepairList* pastPrepareList = nullptr;
    DatabaseConnector databaseConnector;

    Product notOwenProduct;

    QVector<Product>* products = new QVector<Product>();
};

#endif // ADDPRESENTER_H
