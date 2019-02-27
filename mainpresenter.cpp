#include "mainpresenter.h"


MainPresenter::MainPresenter(QObject *parent) : QObject(parent)
{
}

void MainPresenter::setMainView(IMainView *value)
{
    mainView = value;
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
    if (mainView != nullptr)
    {
        mainView->setMethods(methods);
        mainView->setStates(states);
        mainView->setRepairers(repairers);
        mainView->setClients(clients);
    }
}
