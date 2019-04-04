#include "databaseconnector.h"
#include <QSqlQuery>
#include <QVariant>
#include "product.h"

bool DatabaseConnector::open()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("repair_cards.db3");
    auto result = db.open();

    if (!result)
        return false;

    return true;
}

QVector<Handbook*> DatabaseConnector::getHandbook(const QString& handbookName)
{
    QSqlQuery query;
    query.exec(QString("SELECT id, name FROM %1 ORDER BY name").arg(handbookName));
    QVector<Handbook*> methods;

    while (query.next())
    {
        auto  hb = new Handbook();
        hb->id = query.value(0).toInt();
        hb->name = query.value(1).toString();
        methods.push_back(hb);
    }
    return methods;
}

QVector<Handbook*> DatabaseConnector::getClients()
{
    QSqlQuery query;
    query.exec("SELECT id, name, address, phone, person FROM clients");
    QVector<Handbook*> clients;

    while (query.next())
    {
        auto hb = new Client();
        int id = query.value(0).toInt();
        hb->id = id;
        hb->name = query.value(1).toString();
        hb->address = query.value(2).toString();
        hb->phone = query.value(3).toString();
        hb->person = query.value(4).toString();
        clients.push_back(hb);
    }
    return clients;
}

RepairCard DatabaseConnector::getLastCard()
{
    updateIds();
    RepairCard card;
    QSqlQuery query;
    query.exec("SELECT c.*, p.name, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name FROM repair_cards c left join products p on c.product_id=p.id left join clients cl on c.client_id=cl.id left join states st on c.state_id=st.id left join repairers rep on c.repairer_id=rep.id ORDER BY id DESC LIMIT 1");
    if (query.first())
        fillCard(card, query);
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
                               " (%1,%2,%3,%4,'%5','%6','%7',%8,'%9','%10','%11','%12',%13,%14,'%15','%16')")
            .arg(card.id).arg(card.repairerId).arg(card.productId).arg(card.clientId)
            .arg(card.receiveFromClientDate.toString("dd.MM.yyyy")).arg(card.readyDate.toString("dd.MM.yyyy")).arg(card.returnDate.toString("dd.MM.yyyy"))
            .arg(card.stateId)
            .arg(card.complaints).arg(card.reason).arg(card.note).arg(card.barCode)
            .arg(card.costForClient).arg(card.costRepair)
            .arg(card.receiveFromFactoryDate.toString("dd.MM.yyyy")).arg(card.sendDate.toString("dd.MM.yyyy"));

    auto result = query.exec(queryString);
    return result;
}

bool DatabaseConnector::addMethods(const QVector<CardMethod> cardMethods)
{
    db.transaction();
    foreach (CardMethod method, cardMethods) {

        QSqlQuery query;
        auto queryString = QString("insert into cards_methods values (%1,%2,'%3')")
                .arg(method.cardId)
                .arg(method.methodId)
                .arg(method.description);

        auto result = query.exec(queryString);
        if (!result)
        {
            db.rollback();
            return false;
        }
    }
    db.commit();

    return true;
}

QVector<CardMethod> DatabaseConnector::getMethods(int cardId)
{
    QSqlQuery query;
    query.exec(QString("SELECT cm.id_method, cm.description, m.name FROM cards_methods cm left join methods m on cm.id_method=m.id where id_card=%1").arg(cardId));
    QVector<CardMethod> methods;

    while (query.next())
    {
        CardMethod m;
        m.cardId = cardId;
        m.methodId = query.value(0).toInt();
        m.description = query.value(1).toString();
        m.methodName = query.value(2).toString();
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
    currentIndex++;
    if (currentIndex >= ids.count() - 1)
        currentIndex = ids.count() - 1;


    return getCardById(ids[currentIndex]);
}

void DatabaseConnector::deleteCard(int id)
{
    db.transaction();
    QSqlQuery query;
    auto queryString = QString("delete from repair_cards where id=%1").arg(id);

    auto result = query.exec(queryString);
    if (!result)
    {
        db.rollback();
        return;
    }

    queryString = QString("delete from cards_methods where id_card=%1").arg(id);

    result = query.exec(queryString);
    if (!result)
    {
        db.rollback();
        return;
    }

    db.commit();

    updateIds(false);
    currentIndex--;
}

bool DatabaseConnector::updateCard(const RepairCard &card)
{
    db.transaction();
    QSqlQuery query;
    auto queryString = QString("update repair_cards"
                               " set repairer_id=%1,product_id=%2,client_id=%3,receive_date='%4',ready_date='%5',return='%6',state_id=%7,complaints='%8',reason='%9',note='%10',bar_code='%11',"
                               " cost_for_client=%12,cost_repair=%13,receive_date2='%14',sendDate='%15'"
                               " where id=%17")
            .arg(card.repairerId).arg(card.productId).arg(card.clientId)
            .arg(card.receiveFromClientDate.toString("dd.MM.yyyy")).arg(card.readyDate.toString("dd.MM.yyyy")).arg(card.returnDate.toString("dd.MM.yyyy"))
            .arg(card.stateId)
            .arg(card.complaints).arg(card.reason).arg(card.note).arg(card.barCode)
            .arg(card.costForClient).arg(card.costRepair)
            .arg(card.receiveFromFactoryDate.toString("dd.MM.yyyy")).arg(card.sendDate.toString("dd.MM.yyyy"))
            .arg(card.id);

    auto result = query.exec(queryString);
    if (!result)
    {
        db.rollback();
        return false;
    }

    queryString = QString("delete from cards_methods where id_card=%1").arg(card.id);

    result = query.exec(queryString);
    if (!result)
    {
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}

void DatabaseConnector::fillCard(RepairCard& card, QSqlQuery& query)
{
    card.id = query.value(0).toInt();
    card.repairerId = query.value(1).toInt();
    card.productId  = query.value(2).toInt();
    card.clientId = query.value(3).toInt();
    card.receiveFromClientDate = QDate::fromString(query.value(4).toString(),"dd.MM.yyyy");
    card.readyDate= QDate::fromString(query.value(5).toString(),"dd.MM.yyyy");
    card.returnDate= QDate::fromString(query.value(6).toString(),"dd.MM.yyyy");
    card.stateId = query.value(7).toInt();
    card.complaints = query.value(8).toString();
    card.reason = query.value(9).toString();
    card.note = query.value(10).toString();
    card.barCode = query.value(11).toString();
    card.costForClient = query.value(12).toInt();
    card.costRepair = query.value(13).toInt();
    card.receiveFromFactoryDate = QDate::fromString(query.value(14).toString(), "dd.MM.yyyy");
    card.sendDate = QDate::fromString(query.value(15).toString(), "dd.MM.yyyy");
    card.productName = query.value(16).toString();
    card.client.phone = query.value(17).toString();
    card.client.person = query.value(18).toString();
    card.client.address = query.value(19).toString();
    card.state = query.value(20).toString();
    card.repairer = query.value(21).toString();
    card.client.name = query.value(22).toString();
    card.allIndexes = ids.count();
    card.currentIndex = currentIndex+1;
}

bool DatabaseConnector::updateIds(bool currentChange)
{
    QSqlQuery query;
    if (!query.exec(QString("select id from repair_cards ORDER BY id")))
        return false;

    ids.clear();
    while (query.next())
        ids.push_back(query.value(0).toInt());
    if (currentChange)
        currentIndex = ids.count()-1;
    return true;
}

RepairCard DatabaseConnector::getCardById(int id)
{
    RepairCard card;
    QSqlQuery query;
    query.exec(QString("SELECT c.*, p.name, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name FROM repair_cards c left join products p on c.product_id=p.id left join clients cl on c.client_id=cl.id left join states st on c.state_id=st.id left join repairers rep on c.repairer_id=rep.id where c.id=%1").arg(id));
    if (query.first())
        fillCard(card, query);
    return card;
}

QVector<RepairCard> DatabaseConnector::getCardsByProductId(int id)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM repair_cards WHERE product_id=%1 ORDER BY receive_date").arg(id));
    QVector<RepairCard> cards;

    while (query.next())
    {
        RepairCard card;
        fillCard(card, query);
        cards.push_back(card);
    }
    return cards;
}

int DatabaseConnector::addHandbook(const Handbook &handbook, const QString &tableName)
{
    QSqlQuery query;
    auto queryString = QString("insert into %1('name') values ('%2')")
            .arg(tableName)
            .arg(handbook.name);


    if (!query.exec(queryString))
        return -1;

    return query.lastInsertId().toInt();
}

bool DatabaseConnector::updateHandbook(const Handbook &handbook, const QString &tableName)
{
    QSqlQuery query;
    auto queryString = QString("update %1 set name='%2' where id=%3")
            .arg(tableName).arg(handbook.name).arg(handbook.id);

    return query.exec(queryString);
}

void DatabaseConnector::deleteHandbook(int id, const QString &tableName)
{
    QSqlQuery query;
    auto queryString = QString("delete from %1 where id=%2").arg(tableName).arg(id);
    query.exec(queryString);
}

int DatabaseConnector::addProduct(const Product &product)
{
    QSqlQuery query;
    auto queryString = QString("insert into products('name','code') values ('%1', '%2')")
            .arg(product.name)
            .arg(product.code);

    if (!query.exec(queryString))
        return -1;

    return query.lastInsertId().toInt();
}

bool DatabaseConnector::updateProduct(const Product &product)
{
    QSqlQuery query;
    auto queryString = QString("update products set name='%1', code='%2' where id=%3")
            .arg(product.name).arg(product.code).arg(product.id);

    return query.exec(queryString);
}

void DatabaseConnector::deleteProduct(int id)
{
    QSqlQuery query;
    auto queryString = QString("delete from products where id=%2").arg(id);
    query.exec(queryString);
}

int DatabaseConnector::addClient(const Client &client)
{
    QSqlQuery query;
    auto queryString = QString("insert into clients('name', 'phone', 'person', 'address') values ('%1', '%2', '%3', '%4')")
            .arg(client.name)
            .arg(client.phone)
            .arg(client.person)
            .arg(client.address);

    if (!query.exec(queryString))
        return -1;

    return query.lastInsertId().toInt();
}

bool DatabaseConnector::updateClient(const Client &client)
{
    QSqlQuery query;
    auto queryString = QString("update clients set name='%1', phone='%2', person='%3', address='%4' where id=%5")
            .arg(client.name).arg(client.phone).arg(client.person).arg(client.address).arg(client.id);

    return query.exec(queryString);
}

void DatabaseConnector::deleteClient(int id)
{
    QSqlQuery query;
    auto queryString = QString("delete from clients where id=%2").arg(id);
    query.exec(queryString);
}

int DatabaseConnector::getEntries(int id, const QString& column, const QString& table)
{
    QSqlQuery query;
    auto queryStr = QString("select count(*) from %1 where %2=%3").arg(table).arg(column).arg(id);
    query.exec(queryStr);
    if (query.first())
        return query.value(0).toInt();
    return 0;
}

QVector<Handbook *> DatabaseConnector::getProducts()
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM products ORDER BY name"));
    QVector<Handbook*> products;

    while (query.next())
    {
        auto  hb = new Product();
        hb->id = query.value(0).toInt();
        hb->code = query.value(1).toString();
        hb->name = query.value(2).toString();
        products.push_back(hb);
    }
    return products;
}

QVector<RepairCard> DatabaseConnector::getCardsByDateAndClient(QDate date, int clientId)
{
    QSqlQuery query;
    auto q = QString("SELECT c.*, p.name, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name FROM repair_cards c left join products p on c.product_id=p.id left join clients cl on c.client_id=cl.id left join states st on c.state_id=st.id left join repairers rep on c.repairer_id=rep.id WHERE"
                     " receive_date='%1' and c.client_id=%2 ORDER BY receive_date").arg(date.toString("dd.MM.yyyy")).arg(clientId);
    query.exec(q);
    QVector<RepairCard> cards;

    while (query.next())
    {
        RepairCard card;
        fillCard(card, query);
        cards.push_back(card);
    }
    return cards;
}

DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
    if (db.isOpen())
        db.close();
}
