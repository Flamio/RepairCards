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
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(edit(int)), this, SLOT(onEdit(int)));
}

void MainPresenter::setAddPresenter(AddPresenter *value)
{
    addPresenter = value;
    connect(addPresenter, &AddPresenter::addComplete, this, &MainPresenter::onAddComplete);
    connect(addPresenter, &AddPresenter::editComplete, this, &MainPresenter::onEditComplete);
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
    addPresenter->getAddView()->setMode(Adding);
    auto repairCard = dbConnector.getLastCard();
    repairCard.id++;
    addPresenter->getAddView()->setCard(repairCard);
    addPresenter->getAddView()->showWindow();
}

void MainPresenter::onDelete(int id)
{
    dbConnector.deleteCard(id);

    auto card = dbConnector.getNextCard();

    auto methods = dbConnector.getMethods(card.id);
    mainView->setCard(card, methods);
}

void MainPresenter::onEdit(int id)
{
    addPresenter->getAddView()->setMode(Editing);
    auto card = dbConnector.getCardById(id);
    auto methods = dbConnector.getMethods(card.id);
    addPresenter->getAddView()->setCard(card,&methods);
    addPresenter->getAddView()->showWindow();
}

void MainPresenter::onEditComplete(int id)
{
    auto card = dbConnector.getCardById(id);
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
