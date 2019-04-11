#include "mainpresenter.h"

MainPresenter::MainPresenter(QObject *parent) : QObject(parent)
{
    printerFactory = new PrinterFactory(&dbConnector, this);
}

MainPresenter::~MainPresenter()
{
    delete printerFactory;
}

void MainPresenter::setMainView(IMainView *value)
{
    mainView = value;
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(navigation(bool)), this, SLOT(onNavigation(bool)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(add()), this, SLOT(onAdd()));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(deleteSignal(int)), this, SLOT(onDelete(int)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(edit(int)), this, SLOT(onEdit(int)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(print(int, PrintType::PrintType&)), this, SLOT(onPrint(int, PrintType::PrintType&)));
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
    mainView->showWindow();
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

void MainPresenter::onCardClicked(int id)
{
    auto dialog = mainView->newDialog();

    auto card = dbConnector.getCardById(id);
    auto methods = dbConnector.getMethods(id);
    dialog->setCard(card, methods);
    dialog->showWindow();
}

void MainPresenter::onPrint(int id, PrintType::PrintType& type)
{
    auto card = dbConnector.getCardById(id);
    auto printer = printerFactory->build(type, card.costForClient == 0);
    if (printer == nullptr)
        return;
    printer->print(id);
}

void MainPresenter::setPastPrepareList(IPastRepairList *value)
{
    pastPrepareList = value;

    connect(dynamic_cast<QObject*>(pastPrepareList), SIGNAL(cardClicked(int)), this, SLOT(onCardClicked(int)));
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
