#include "addpresenter.h"
#include "producteditform.h"

AddPresenter::AddPresenter(QObject *parent) : QObject(parent)
{
}

void AddPresenter::setAddView(IAddView *value)
{
    addView = value;
    connect(dynamic_cast<QObject*>(addView), SIGNAL(barCodeFinish(QString, bool)), this, SLOT(onBarCodeFinish(QString, bool)));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(addSignal(const RepairCard&, const QVector<CardMethod>&)), this, SLOT(onAdd(const RepairCard&, const QVector<CardMethod>&)));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editSignal(const RepairCard&, const QVector<CardMethod>&)), this, SLOT(onEdit(const RepairCard&, const QVector<CardMethod>&)));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editRepairers()), this, SLOT(onEditRepairers()));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editMethods()), this, SLOT(onEditMethods()));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editClients()), this, SLOT(onEditClients()));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editProducts()), this, SLOT(onEditProducts()));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(showProdictSearch()), this, SLOT(onShowProdictSearch()));
}

void AddPresenter::setDatabaseConnector(const DatabaseConnector &value)
{
    databaseConnector = value;
}

void AddPresenter::start()
{
    auto methods = databaseConnector.getHandbook("methods");
    auto repairers = databaseConnector.getHandbook("repairers");
    auto states = databaseConnector.getHandbook("states");
    auto clients = databaseConnector.getClients();
    auto products = databaseConnector.getProducts();

    if (addView == nullptr)
        return;

    addView->setMethods(methods);
    addView->setStates(states);
    addView->setRepairers(repairers);
    addView->setClients(clients);

    repairerEditView->setHandbooks(repairers);
    methodEditView->setHandbooks(methods);
    clientEditView->setHandbooks(clients);
}

void AddPresenter::onBarCodeFinish(QString code, bool isOwen)
{
    if (isOwen)
    {
        auto productCode = code.left(5);
        auto product = databaseConnector.getProductByCode(productCode);
        addView->setProduct(product);
        checkPastRepairs(product.id, code);
        return;
    }
    checkPastRepairs(notOwenProduct.id, code);
}

void AddPresenter::onAdd(const RepairCard &card, const QVector<CardMethod> &methods)
{
    auto added = databaseConnector.addCard(card);
    if (!added)
    {
        addView->showInfo("Ошибка добавления ремонтной карты");
        return;
    }

    added = databaseConnector.addMethods(methods);
    if (!added)
    {
        addView->showInfo("Ошибка добавления способов устранения!");
        return;
    }

    addComplete();
    addView->closeWindow();
}

void AddPresenter::onEdit(const RepairCard &card, const QVector<CardMethod> &methods)
{
    auto result = databaseConnector.updateCard(card);
    if (!result)
    {
        addView->showInfo("Ошибка обновления ремонтной карты !");
        return;
    }

    result = databaseConnector.addMethods(methods);
    if (!result)
    {
        addView->showInfo("Ошибка добавления способов устранения!");
        return;
    }

    editComplete(card.id);
    addView->closeWindow();
}

void AddPresenter::onEditRepairers()
{
    repairerEditView->showWindow();
}

void AddPresenter::onEditMethods()
{
    methodEditView->showWindow();
}

void AddPresenter::onMethodAdd(Handbook *h)
{
    auto addedId = databaseConnector.addHandbook(*h, "methods");
    if (addedId == -1)
    {
        addView->showInfo("Не удалось добавить новый метод!");
        return;
    }

    h->id = addedId;
    auto methods = databaseConnector.getHandbook("methods");
    addView->setMethods(methods);
    addView->addMethod(*h);
    methodEditView->setMode(Editing);
    methodEditView->setHandbooks(methods);
    methodEditView->closeWindow();
}

void AddPresenter::onMethodEdit(Handbook *h)
{
    auto entries = databaseConnector.getEntries(h->id, "id_method", "cards_methods");
    if (entries != 0)
    {
        addView->showInfo("Нельзя редактировать! Этот способ устранения используется в других ремонтных картах!");
        return;
    }

    databaseConnector.updateHandbook(*h, "methods");
    auto methods = databaseConnector.getHandbook("methods");
    addView->setMethods(methods);
    methodEditView->setHandbooks(methods);
    methodEditView->setHandbook(h->id);
    methodEditView->closeWindow();
}

void AddPresenter::onDeleteMethod(int id)
{
    auto entries = databaseConnector.getEntries(id,"id_method", "cards_methods");
    if (entries != 0)
    {
        addView->showInfo("Нельзя удалить! Этот способ устранения используется в других ремонтных картах!");
        return;
    }
    databaseConnector.deleteHandbook(id, "methods");
    auto methods = databaseConnector.getHandbook("methods");
    methodEditView->setHandbooks(methods);
    addView->setMethods(methods);
}

void AddPresenter::onEditClients()
{
    clientEditView->showWindow();
}

void AddPresenter::onEditProducts()
{
    auto product = databaseConnector.getLastProduct();
    auto view =  (ProductEditForm*)productEditView;
    view->setProduct(product);
    productEditView->showWindow();
}

void AddPresenter::onRepairerAdd(Handbook *h)
{
    auto addedId = databaseConnector.addHandbook(*h, "repairers");
    if (addedId == -1)
    {
        addView->showInfo("Не удалось добавить новый метод!");
        return;
    }

    h->id = addedId;
    auto repairers = databaseConnector.getHandbook("repairers");
    addView->setRepairers(repairers);
    addView->setRepairer(addedId);
    repairerEditView->setMode(Editing);
    repairerEditView->setHandbooks(repairers);
    repairerEditView->closeWindow();
}

void AddPresenter::onRepairerEdit(Handbook *h)
{
    auto entries = databaseConnector.getEntries(h->id, "repairer_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя редактировать! Этот ремонтник используется в других ремонтных картах!");
        return;
    }

    databaseConnector.updateHandbook(*h, "repairers");
    auto repairers = databaseConnector.getHandbook("repairers");
    addView->setRepairers(repairers);
    addView->setRepairer(h->id);
    repairerEditView->setHandbooks(repairers);
    repairerEditView->setHandbook(h->id);
    repairerEditView->closeWindow();
}

void AddPresenter::onDeleteRepairer(int id)
{
    auto entries = databaseConnector.getEntries(id, "repairer_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя удалять! Этот ремонтник используется в других ремонтных картах!");
        return;
    }
    databaseConnector.deleteHandbook(id, "repairers");
    auto repairers = databaseConnector.getHandbook("repairers");
    repairerEditView->setHandbooks(repairers);
    addView->setRepairers(repairers);
}

void AddPresenter::onProductAdd(Handbook *h)
{
    auto product = (Product*)h;

    if (product->isOwen && product->code.isEmpty())
    {
        addView->showInfo("Пустой код изделия!");
        return;
    }

    auto productsEntries = databaseConnector.getProductCountWithTheSameCode(product->code, -1);
    if (productsEntries > 0)
    {
        addView->showInfo("Изделие с таким номером уже существует!");
        return;
    }

    auto addedId = databaseConnector.addProduct(*product);
    if (addedId == -1)
    {
        addView->showInfo("Не удалось добавить новое изделие!");
        return;
    }

    h->id = addedId;
    productEditView->setMode(Editing);
    productEditView->closeWindow();
    if (product->isOwen)
        addView->barCodeFinishEmit();
    else
    {
        notOwenProduct = *product;
        addView->setProduct(*product);
    }
}

void AddPresenter::onProductEdit(Handbook *h)
{
    if (h->id == 0)
    {
        addView->showInfo("Нет такого изделия!");
        return;
    }

    auto entries = databaseConnector.getEntries(h->id, "product_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя редактировать! Это изделие используется в других ремонтных картах!");
        return;
    }

    auto product = (Product*)h;

    if (product->isOwen && product->code.isEmpty())
    {
        addView->showInfo("Пустой код изделия!");
        return;
    }

    auto productsEntries = databaseConnector.getProductCountWithTheSameCode(product->code, product->id);
    if (productsEntries > 0)
    {
        addView->showInfo("Изделие с таким номером уже существует!");
        return;
    }

    databaseConnector.updateProduct(*product);
    productEditView->closeWindow();
    addView->setProduct(*product);
    addView->barCodeFinishEmit();
}

void AddPresenter::onDeleteProduct(int id)
{
    if (id == 0)
    {
        addView->showInfo("Нет такого изделия!");
        return;
    }

    auto entries = databaseConnector.getEntries(id, "product_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя удалять! Это изделие используется в других ремонтных картах!");
        return;
    }
    databaseConnector.deleteProduct(id);
    Product dum;
    addView->setProduct(dum);
    addView->barCodeFinishEmit();
    productEditView->closeWindow();
}

void AddPresenter::onClientAdd(Handbook *h)
{
    auto client = (Client*)h;

    auto addedId = databaseConnector.addClient(*client);
    if (addedId == -1)
    {
        addView->showInfo("Не удалось добавить нового клиента!");
        return;
    }

    h->id = addedId;
    auto clients = databaseConnector.getClients();
    addView->setClients(clients);
    addView->setClient(client->id);
    clientEditView->setMode(Editing);
    clientEditView->setHandbooks(clients);
    clientEditView->closeWindow();
}

void AddPresenter::onClientEdit(Handbook *h)
{
    auto client = (Client*)h;
    auto entries = databaseConnector.getEntries(h->id, "client_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя редактировать! Этот клиент используется в других ремонтных картах!");
        return;
    }

    databaseConnector.updateClient(*client);
    auto clients = databaseConnector.getClients();
    addView->setClients(clients);
    addView->setClient(client->id);
    clientEditView->setHandbooks(clients);
    clientEditView->setHandbook(h->id);
    clientEditView->closeWindow();
}

void AddPresenter::onDeleteClient(int id)
{
    auto entries = databaseConnector.getEntries(id, "client_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя удалять! Этот клиент используется в других ремонтных картах!");
        return;
    }
    databaseConnector.deleteClient(id);
    auto clients = databaseConnector.getClients();
    clientEditView->setHandbooks(clients);
    addView->setClients(clients);
}

void AddPresenter::checkPastRepairs(int productId, const QString& barcode)
{
    if (addView->getMode() == Editing)
        return;
    auto pastCards = databaseConnector.getCardsByProductIdAndBarcode(productId, barcode.trimmed());
    if (pastCards.count() == 0)
        return;

    pastPrepareList->setCards(pastCards);
    pastPrepareList->showWindow();
}

void AddPresenter::onShowProdictSearch()
{
    productSearch->showView();
}

void AddPresenter::onProductSearchDone()
{
    auto product = productSearch->getProduct();
    if (product->id == 0)
    {
        addView->showInfo("Изделие не найдено!");
        return;
    }
    notOwenProduct = *product;
    addView->setProduct(*product);
    productSearch->closeView();
}

void AddPresenter::setProductSearch(ProductSearchPresenter *value)
{
    productSearch = value;
    connect(productSearch, &ProductSearchPresenter::done, this, &AddPresenter::onProductSearchDone);
}

void AddPresenter::setPastPrepareList(IPastRepairList *value)
{
    pastPrepareList = value;
}

void AddPresenter::setProductEditView(IHandbookEditView *value)
{
    productEditView = value;
    connect(dynamic_cast<QObject*>(productEditView), SIGNAL(add(Handbook*)), this, SLOT(onProductAdd(Handbook*)));
    connect(dynamic_cast<QObject*>(productEditView), SIGNAL(edit(Handbook*)), this, SLOT(onProductEdit(Handbook*)));
    connect(dynamic_cast<QObject*>(productEditView), SIGNAL(deleteHandbook(int)), this, SLOT(onDeleteProduct(int)));
}

void AddPresenter::setClientEditView(IHandbookEditView *value)
{
    clientEditView = value;
    connect(dynamic_cast<QObject*>(clientEditView), SIGNAL(add(Handbook*)), this, SLOT(onClientAdd(Handbook*)));
    connect(dynamic_cast<QObject*>(clientEditView), SIGNAL(edit(Handbook*)), this, SLOT(onClientEdit(Handbook*)));
    connect(dynamic_cast<QObject*>(clientEditView), SIGNAL(deleteHandbook(int)), this, SLOT(onDeleteClient(int)));
}

void AddPresenter::setMethodEditView(IHandbookEditView *value)
{
    methodEditView = value;
    connect(dynamic_cast<QObject*>(methodEditView), SIGNAL(add(Handbook*)), this, SLOT(onMethodAdd(Handbook*)));
    connect(dynamic_cast<QObject*>(methodEditView), SIGNAL(edit(Handbook*)), this, SLOT(onMethodEdit(Handbook*)));
    connect(dynamic_cast<QObject*>(methodEditView), SIGNAL(deleteHandbook(int)), this, SLOT(onDeleteMethod(int)));
}

void AddPresenter::setRepairerEditView(IHandbookEditView *value)
{
    repairerEditView = value;

    connect(dynamic_cast<QObject*>(repairerEditView), SIGNAL(add(Handbook*)), this, SLOT(onRepairerAdd(Handbook*)));
    connect(dynamic_cast<QObject*>(repairerEditView), SIGNAL(edit(Handbook*)), this, SLOT(onRepairerEdit(Handbook*)));
    connect(dynamic_cast<QObject*>(repairerEditView), SIGNAL(deleteHandbook(int)), this, SLOT(onDeleteRepairer(int)));
}

IAddView *AddPresenter::getAddView() const
{
    return addView;
}
