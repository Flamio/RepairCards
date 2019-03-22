#include "addpresenter.h"


AddPresenter::AddPresenter(QObject *parent) : QObject(parent)
{
}

void AddPresenter::setAddView(IAddView *value)
{
    addView = value;
    connect(dynamic_cast<QObject*>(addView), SIGNAL(barCodeFinish(QString)), this, SLOT(onBarCodeFinish(QString)));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(addSignal(const RepairCard&, const QVector<CardMethod>&)), this, SLOT(onAdd(const RepairCard&, const QVector<CardMethod>&)));    
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editSignal(const RepairCard&, const QVector<CardMethod>&)), this, SLOT(onEdit(const RepairCard&, const QVector<CardMethod>&)));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editRepairers()), this, SLOT(onEditRepairers()));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(editMethods()), this, SLOT(onEditMethods()));
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

    if (addView == nullptr)
        return;

    addView->setMethods(methods);
    addView->setStates(states);
    addView->setRepairers(repairers);
    addView->setClients(clients);

    if (repairerEditView == nullptr)
        return;

    repairerEditView->setHandbooks(repairers);

    if (methodEditView == nullptr)
        return;

    methodEditView->setHandbooks(methods);

    /*repairCard = databaseConnector.getLastCard();
    auto methods_ = databaseConnector.getMethods(repairCard.id);
    if (repairCard.id == 0)
    {
        addView->setMode(Adding);
        addView->showInfo("Не найдено ни одной ремонтной карты! Создайте новую");
        repairCard.id++;
    }
    addView->setCard(repairCard,methods_);*/
}

void AddPresenter::onBarCodeFinish(QString code)
{
    auto productCode = code.left(5);
    auto product = databaseConnector.getProductByCode(productCode);
    addView->setProduct(product);
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

void AddPresenter::onMethodAdd(Handbook &h)
{
    auto addedId = databaseConnector.addHandbook(h, "methods");
    if (addedId == -1)
    {
        addView->showInfo("Не удалось добавить новый метод!");
        return;
    }

    h.id = addedId;
    auto methods = databaseConnector.getHandbook("methods");
    addView->setMethods(methods);
    addView->addMethod(h);
    methodEditView->setMode(Editing);
    methodEditView->setHandbooks(methods);
    methodEditView->closeWindow();
}

void AddPresenter::onMethodEdit(const Handbook &h)
{
    auto entries = databaseConnector.getEntries(h.id, "id_methods", "card_methods");
    if (entries != 0)
    {
        addView->showInfo("Нельзя редактировать! Этот способ устранения используется в других ремонтных картах!");
        return;
    }

    databaseConnector.updateHandbook(h, "methods");
    auto methods = databaseConnector.getHandbook("methods");
    addView->setMethods(methods);
    methodEditView->setHandbooks(methods);
    methodEditView->setHandbook(h.id);
    methodEditView->closeWindow();
}

void AddPresenter::onDeleteMethod(int id)
{
    auto entries = databaseConnector.getEntries(id,"id_methods", "card_methods");
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

void AddPresenter::onRepairerAdd(Handbook &h)
{
    auto addedId = databaseConnector.addHandbook(h, "repairers");
    if (addedId == -1)
    {
        addView->showInfo("Не удалось добавить новый метод!");
        return;
    }

    h.id = addedId;
    auto repairers = databaseConnector.getHandbook("repairers");
    addView->setRepairers(repairers);
    addView->setRepairer(addedId);
    repairerEditView->setMode(Editing);
    repairerEditView->setHandbooks(repairers);
    repairerEditView->closeWindow();
}

void AddPresenter::onRepairerEdit(const Handbook &h)
{
    auto entries = databaseConnector.getEntries(h.id, "repairer_id", "repair_cards");
    if (entries != 0)
    {
        addView->showInfo("Нельзя редактировать! Этот ремонтник используется в других ремонтных картах!");
        return;
    }

    databaseConnector.updateHandbook(h, "repairers");
    auto repairers = databaseConnector.getHandbook("repairers");
    addView->setRepairers(repairers);
    addView->setRepairer(h.id);
    repairerEditView->setHandbooks(repairers);
    repairerEditView->setHandbook(h.id);
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

void AddPresenter::setMethodEditView(IHandbookEditView *value)
{
    methodEditView = value;
    connect(dynamic_cast<QObject*>(methodEditView), SIGNAL(add(Handbook&)), this, SLOT(onMethodAdd(Handbook&)));
    connect(dynamic_cast<QObject*>(methodEditView), SIGNAL(edit(const Handbook&)), this, SLOT(onMethodEdit(const Handbook&)));
    connect(dynamic_cast<QObject*>(methodEditView), SIGNAL(deleteHandbook(int)), this, SLOT(onDeleteMethod(int)));
}

void AddPresenter::setRepairerEditView(IHandbookEditView *value)
{
    repairerEditView = value;

    connect(dynamic_cast<QObject*>(repairerEditView), SIGNAL(add(Handbook&)), this, SLOT(onRepairerAdd(Handbook&)));
    connect(dynamic_cast<QObject*>(repairerEditView), SIGNAL(edit(const Handbook&)), this, SLOT(onRepairerEdit(const Handbook&)));
    connect(dynamic_cast<QObject*>(repairerEditView), SIGNAL(deleteHandbook(int)), this, SLOT(onDeleteRepairer(int)));
}

IAddView *AddPresenter::getAddView() const
{
    return addView;
}
