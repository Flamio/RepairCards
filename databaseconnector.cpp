#include "databaseconnector.h"
#include <QSqlQuery>
#include <QVariant>

bool DatabaseConnector::open()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("repair_cards.db3");
    auto result = db.open();

    if (!result)
        return false;

    QSqlQuery query;
    if (!query.exec(QString("select id from repair_cards ORDER BY id")))
        return false;

    ids.clear();
    while (query.next())
        ids.push_back(query.value(0).toInt());

    currentIndex = ids.count()-1;
    return true;
}

QVector<Handbook> DatabaseConnector::getHandbook(const QString& handbookName)
{
    QSqlQuery query;
    query.exec(QString("SELECT id, name FROM %1 ORDER BY name").arg(handbookName));
    QVector<Handbook> methods;

    while (query.next())
    {
        Handbook hb;
        hb.id = query.value(0).toInt();
        hb.name = query.value(1).toString();
        methods.push_back(hb);
    }
    return methods;
}

QHash<int,Client> DatabaseConnector::getClients()
{
    QSqlQuery query;
    query.exec("SELECT id, name, address FROM clients");
    QHash<int,Client> clients;

    while (query.next())
    {
        Client hb;
        int id = query.value(0).toInt();
        hb.id = id;
        hb.name = query.value(1).toString();
        hb.address = query.value(2).toString();
        clients[id] = hb;
    }
    return clients;
}

RepairCard DatabaseConnector::getLastCard()
{
    RepairCard card;
    QSqlQuery query;
    query.exec("SELECT c.*, p.name FROM repair_cards c left join products p on c.product_id=p.id ORDER BY id DESC LIMIT 1");
    if (query.first())
        fillCard(card, query);
    currentIndex = ids.count() - 1;
    return card;
}

Handbook DatabaseConnector::getProductByCode(const QString &code)
{
    Handbook product;
    QSqlQuery query;
    int codeInt = code.toInt();
    query.exec(QString("SELECT id, name FROM products where code = '%1'").arg(codeInt));
    if (query.first())
    {
        product.id = query.value(0).toInt();
        product.name = query.value(1).toString();
    }
    return product;
}

bool DatabaseConnector::addCard(const RepairCard &card)
{
    QSqlQuery query;
    auto queryString = QString("insert into repair_cards values"
                               " (%1,%2,%3,%4,'%5','%6','%7',%8,'%9','%10','%11','%12',%13,%14)")
            .arg(card.id).arg(card.repairerId).arg(card.productId).arg(card.clientId)
            .arg(card.receiveDate.toString("dd.MM.yyyy")).arg(card.readyDate.toString("dd.MM.yyyy")).arg(card.returnDate.toString("dd.MM.yyyy"))
            .arg(card.stateId)
            .arg(card.complaints).arg(card.reason).arg(card.note).arg(card.barCode)
            .arg(card.costForClient).arg(card.costRepair);

    auto result = query.exec(queryString);
    if (result)
    {
        ids.push_back(card.id);
        currentIndex = ids.count() - 1;
    }
    return result;
}

bool DatabaseConnector::addMethods(const QVector<CardMethod> cardMethods)
{
    foreach (CardMethod method, cardMethods) {

        QSqlQuery query;
        auto queryString = QString("insert into cards_methods values (%1,%2,'%3')")
                .arg(method.cardId)
                .arg(method.methodId)
                .arg(method.description);

        auto result = query.exec(queryString);
        if (!result)
            return false;
    }

    return true;
}

QVector<CardMethod> DatabaseConnector::getMethods(int cardId)
{
    QSqlQuery query;
    query.exec(QString("SELECT id_method, description FROM cards_methods where id_card=%1").arg(cardId));
    QVector<CardMethod> methods;

    while (query.next())
    {
        CardMethod m;
        m.cardId = cardId;
        m.methodId = query.value(0).toInt();
        m.description = query.value(1).toString();
        methods.push_back(m);
    }

    return methods;
}

RepairCard DatabaseConnector::getPreviousCard()
{
    if (currentIndex != 0)
        currentIndex--;

    return getCardById(ids[currentIndex]);
}

RepairCard DatabaseConnector::getNextCard()
{
    if (currentIndex != ids.count() - 1)
        currentIndex++;

    return getCardById(ids[currentIndex]);
}

void DatabaseConnector::fillCard(RepairCard& card, QSqlQuery& query)
{
    card.id = query.value(0).toInt();
    card.repairerId = query.value(1).toInt();
    card.productId  = query.value(2).toInt();
    card.clientId = query.value(3).toInt();
    card.receiveDate = QDate::fromString(query.value(4).toString(),"dd.MM.yyyy");
    card.readyDate= QDate::fromString(query.value(5).toString(),"dd.MM.yyyy");;
    card.returnDate= QDate::fromString(query.value(6).toString(),"dd.MM.yyyy");
    card.stateId = query.value(7).toInt();
    card.complaints = query.value(8).toString();
    card.reason = query.value(9).toString();
    card.note = query.value(10).toString();
    card.barCode = query.value(11).toString();
    card.costForClient = query.value(12).toInt();
    card.costRepair = query.value(13).toInt();
    card.productName = query.value(14).toString();
    card.allIndexes = ids.count();
    card.currentIndex = currentIndex+1;
}

RepairCard DatabaseConnector::getCardById(int id)
{
    RepairCard card;
    QSqlQuery query;
    query.exec(QString("SELECT c.*, p.name FROM repair_cards c left join products p on c.product_id=p.id where c.id=%1").arg(id));
    if (query.first())
        fillCard(card, query);
    return card;
}

DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
    if (db.isOpen())
        db.close();
}
