#include "addpresenter.h"


AddPresenter::AddPresenter(QObject *parent) : QObject(parent)
{
}

void AddPresenter::setAddView(IAddView *value)
{
    addView = value;
    connect(dynamic_cast<QObject*>(addView), SIGNAL(barCodeFinish(QString)), this, SLOT(onBarCodeFinish(QString)));
    connect(dynamic_cast<QObject*>(addView), SIGNAL(addSignal(const RepairCard&, const QVector<CardMethod>&)), this, SLOT(onAdd(const RepairCard&, const QVector<CardMethod>&)));    
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

void AddPresenter::show()
{
    repairCard = databaseConnector.getLastCard();
    repairCard.id++;
    addView->setCard(repairCard);
    addView->showWindow();
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

    complete();
    addView->closeWindow();
}
