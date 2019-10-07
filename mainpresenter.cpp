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
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(showSendedProducts()), this, SLOT(onShowSendedProducts()));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(showExtremeCard(ExtremeCardType::ExtremeCardType)), this, SLOT(onShowExtremeCard(ExtremeCardType::ExtremeCardType)));
    connect(dynamic_cast<QObject*>(mainView), SIGNAL(showCardByIndex(int)), this, SLOT(onShowCardByIndex(int)));

    Callbacks callbacks;
    callbacks.searchCards = [=] (){
        this->cardSearchView->showWindow();
    };
    mainView->setCallbacks(callbacks);
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
    if (card.stateId != 0)
    {
        auto methods = dbConnector.getMethods(card.id);
        mainView->setCard(card, methods);
        mainView->showWindow();
        return;
    }

    addPresenter->getAddView()->showWindow();
    addPresenter->getAddView()->showInfo("В базе данных нет ни одной ремонтной карты, создайте новую");
    RepairCard cardNew;
    cardNew.id++;
    addPresenter->getAddView()->setCard(cardNew);
    mainView->closeWindow();
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

    if (card.state == "")
    {
        showLastCard();
        return;
    }

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

void MainPresenter::onShowSendedProducts()
{
    auto cards = dbConnector.getSendedCards();
    if (cards.size() == 0)
    {
        addPresenter->getAddView()->showInfo("Нет отправленных изделий!");
        return;
    }

    pastPrepareList->setCards(cards);
    pastPrepareList->showWindow("Эти изделия были отправлены:");
}

void MainPresenter::onShowExtremeCard(ExtremeCardType::ExtremeCardType cardType)
{
    RepairCard card;
    if (cardType == ExtremeCardType::Last)
        card = dbConnector.getLastCard();
    else
        card = dbConnector.getFirstCard();

    auto methods = dbConnector.getMethods(card.id);
    mainView->setCard(card, methods);
}

void MainPresenter::onShowCardByIndex(int index)
{
    auto card = dbConnector.getCardByIndex(index);
    auto methods = dbConnector.getMethods(card.id);
    mainView->setCard(card,methods);
}

void MainPresenter::setCardSearchView(IHandbookSearchView *value)
{
    cardSearchView = value;

    HandbookSearchCallbacks c;
    c.done = [=](int index)
    {
        auto card = (*cards)[index];
        auto methods = dbConnector.getMethods(card.id);
        mainView->setCard(card, methods);
        dbConnector.setCurrentIndex(card.id);
        cardSearchView->closeWindow();
    };

    c.searchHandbook = [=](const QString& name, QMap<QString, QVariant>& options)
    {        
        bool issuedProducts = options["issued_products"].toBool();
        (*cards) = dbConnector.getRepairCardsByProductNameOrCode(name, issuedProducts);

        QVector<Handbook>h;

        for (auto c : *cards)
            h.append(c);

        cardSearchView->setHandbooks(h);
    };

    cardSearchView->setCallbacks(c);
    cardSearchView->setOption("check", "issued_products", "Выданные изделия");
    cardSearchView->setQueryTitle("Строка поиска");
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
