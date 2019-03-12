#include "mainpresenter.h"

MainPresenter::MainPresenter(QObject *parent) : QObject(parent)
{

}

void MainPresenter::setMainView(IMainView *value)
{
    mainView = value;
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(navigation(bool)), this, SLOT(onNavigation(bool)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(add()), this, SLOT(onAdd()));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(deleteSignal(int)), this, SLOT(onDelete(int)));
}

void MainPresenter::setAddPresenter(AddPresenter *value)
{
    addPresenter = value;
    connect(addPresenter, &AddPresenter::complete, this, &MainPresenter::onAddComplete);
}

void MainPresenter::setDbConnector(const DatabaseConnector &value)
{
    dbConnector = value;
}

void MainPresenter::showLastCard()
{
    auto card = dbConnector.getLastCard();
    auto methods = dbConnector.getMethods(card.id);
    mainView->setCard(card, methods);
}

void MainPresenter::start()
{
    showLastCard();
}

void MainPresenter::onAdd()
{
    addPresenter->show();
}

void MainPresenter::onDelete(int id)
{
    dbConnector.deleteCard(id);

    auto card = dbConnector.getNextCard();

    auto methods = dbConnector.getMethods(card.id);
    mainView->setCard(card, methods);
}

void MainPresenter::onNavigation(bool forward)
{
    RepairCard card;
    if (!forward)
        card = dbConnector.getPreviousCard();
    else
        card = dbConnector.getNextCard();

    auto methods = dbConnector.getMethods(card.id);
    mainView->setCard(card, methods);
}

void MainPresenter::onAddComplete()
{
    showLastCard();
}
