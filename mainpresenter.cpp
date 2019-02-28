#include "mainpresenter.h"


MainPresenter::MainPresenter(QObject *parent) : QObject(parent)
{
}

void MainPresenter::setMainView(IMainView *value)
{
    mainView = value;
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(barCodeFinish(QString)), this, SLOT(onBarCodeFinish(QString)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(addSignal(const RepairCard&, const QVector<CardMethod>&)), this, SLOT(onAdd(const RepairCard&, const QVector<CardMethod>&)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(newCard()), this, SLOT(onNewCard()));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(cancelAdding()), this, SLOT(onCancelAdding()));
}

void MainPresenter::setDatabaseConnector(const DatabaseConnector &value)
{
    databaseConnector = value;
}

void MainPresenter::start()
{
    auto methods = databaseConnector.getHandbook("methods");
    auto repairers = databaseConnector.getHandbook("repairers");
    auto states = databaseConnector.getHandbook("states");
    auto clients = databaseConnector.getClients();

    if (mainView == nullptr)
        return;

    mainView->setMethods(methods);
    mainView->setStates(states);
    mainView->setRepairers(repairers);
    mainView->setClients(clients);

    repairCard = databaseConnector.getLastCard();
    auto methods_ = databaseConnector.getMethods(repairCard.id);
    if (repairCard.id == 0)
    {
        mainView->setMode(Adding);
        mainView->showInfo("Не найдено ни одной ремонтной карты! Создайте новую");
        repairCard.id++;
    }
    mainView->setCard(repairCard,methods_);
}

void MainPresenter::onBarCodeFinish(QString code)
{
    auto productCode = code.left(5);
    auto product = databaseConnector.getProductByCode(productCode);
    mainView->setProduct(product);
}

void MainPresenter::onAdd(const RepairCard &card, const QVector<CardMethod> &methods)
{
    auto added = databaseConnector.addCard(card);
    if (!added)
    {
        mainView->showInfo("Ошибка добавления ремонтной карты");
        return;
    }

    added = databaseConnector.addMethods(methods);
    if (!added)
    {
        mainView->showInfo("Ошибка добавления способов устранения!");
        return;
    }
    repairCard = databaseConnector.getLastCard();
    auto methods_ = databaseConnector.getMethods(repairCard.id);
    mainView->setMode(Reading);
    mainView->setCard(repairCard,methods_);
}

void MainPresenter::onNewCard()
{
    auto lastCard = databaseConnector.getLastCard();
    RepairCard newCard;
    newCard.id = lastCard.id+1;
    mainView->setMode(Adding);
    QVector<CardMethod> methods;
    mainView->setCard(newCard,methods);
}

void MainPresenter::onCancelAdding()
{
    auto lastCard = databaseConnector.getLastCard();
    auto methods = databaseConnector.getMethods(repairCard.id);

    mainView->setMode(Reading);
    mainView->setCard(lastCard, methods);
}
