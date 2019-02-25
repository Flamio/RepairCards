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
    auto methods = databaseConnector.getHandbook("methods");
    auto repairers = databaseConnector.getHandbook("repairers");
    auto states = databaseConnector.getHandbook("states");
    if (mainView != nullptr)
    {
        mainView->setMethods(methods);
        mainView->setStates(states);
        mainView->setRepairers(repairers);
    }
}
