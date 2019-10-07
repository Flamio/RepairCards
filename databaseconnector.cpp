#include "databaseconnector.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include "product.h"
#include <QFile>
#include <QTextStream>
#include <QDirIterator>
#include <QDebug>

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
    query.exec("SELECT id, name, address, phone, person FROM clients ORDER BY name");
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
    query.exec("SELECT c.*, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name, p.* FROM repair_cards c left join products p on c.product_id=p.id left join clients cl on c.client_id=cl.id left join states st on c.state_id=st.id left join repairers rep on c.repairer_id=rep.id ORDER BY id DESC LIMIT 1");
    if (query.first())
        fillCard(card, query);
    return card;
}

Product DatabaseConnector:: getLastProduct()
{
    Product product;
    QSqlQuery query;
    query.exec("SELECT * FROM products ORDER BY id DESC LIMIT 1");
    if (query.first())
    {
        product.id = query.value(0).toInt();
        product.code = query.value(1).toString();
        product.name = query.value(2).toString();
        product.isOwen = query.value(3).toBool();
    }
    return product;
}

Product DatabaseConnector::getProductByCode(const QString &code)
{
    Product product;
    QSqlQuery query;
    int codeInt = code.toInt();
    query.exec(QString("SELECT id, name, code, isOwen FROM products where code = '%1'").arg(codeInt));
    if (query.first())
    {
        product.id = query.value(0).toInt();
        product.name = query.value(1).toString();
        product.code = query.value(2).toString();
        product.isOwen = query.value(3).toBool();
    }
    return product;
}

QVector<Product> DatabaseConnector::getProductsByName(const QString &name, bool isNotOwenOnly)
{
    QVector<Product> products;
    QSqlQuery query;
    auto q = QString("SELECT id, name, code, isOwen FROM products where lower(name) like lower('%%1%')")
            .arg(name);

    if (isNotOwenOnly)
        q += " and isOwen=0 ORDER BY name  LIMIT 300";
    else
        q += " ORDER BY name LIMIT 300";

    query.exec(q);
    while (query.next())
    {
        Product product;
        product.id = query.value(0).toInt();
        product.name = query.value(1).toString();
        product.code = query.value(2).toString();
        product.isOwen = query.value(3).toBool();
        products.push_back(product);
    }
    return products;
}

int DatabaseConnector::getProductCountWithTheSameCode(const QString &code, int id)
{
    if (code.isEmpty())
        return 0;
    QSqlQuery query;
    int codeInt = code.toInt();
    query.exec(QString("SELECT count(*) FROM products where code = '%1' and id != %2").arg(codeInt).arg(id));
    if (query.first())
        return query.value(0).toInt();
    return 0;
}

bool DatabaseConnector::addCard(const RepairCard &card)
{
    QString year = card.year.trimmed() == "20" || card.year.trimmed().isEmpty() ? "NULL" : card.year;
    QString month = card.month.trimmed() == "" ? "NULL" : card.month;
    QSqlQuery query;
    auto queryString = QString("insert into repair_cards values"
                               " (%1,%2,%3,%4,'%5','%6','%7',%8,'%9','%10','%11','%12',%13,%14,'%15','%16', %17, %18)")
            .arg(card.id).arg(card.repairerId).arg(card.product.id).arg(card.clientId)
            .arg(card.receiveFromClientDate.toString("dd.MM.yyyy")).arg(card.readyDate.toString("dd.MM.yyyy")).arg(card.returnDate.toString("dd.MM.yyyy"))
            .arg(card.stateId)
            .arg(card.complaints).arg(card.reason).arg(card.note).arg(card.barCode)
            .arg(card.costForClient).arg(card.costRepair)
            .arg(card.receiveFromFactoryDate.toString("dd.MM.yyyy")).arg(card.sendDate.toString("dd.MM.yyyy"))
            .arg(year).arg(month);

    auto result = query.exec(queryString);
    if (!result)
        lastError = query.lastError().text();
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
            lastError = db.lastError().text();
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
    if (ids.count() == 0)
        return bufCard;

    currentIndex++;
    if (currentIndex >= ids.count() - 1)
        currentIndex = ids.count() - 1;


    return getCardById(ids[currentIndex]);
}

RepairCard DatabaseConnector::getFirstCard()
{
    currentIndex = 0;
    return getCardById(ids[currentIndex]);
}

void DatabaseConnector::deleteCard(int id)
{
    db.transaction();
    QSqlQuery query;


    auto queryString = QString("delete from cards_methods where id_card=%1").arg(id);

    auto result = query.exec(queryString);
    if (!result)
    {
        db.rollback();
        return;
    }

    queryString = QString("delete from repair_cards where id=%1").arg(id);

    result = query.exec(queryString);
    if (!result)
    {
        lastError = query.lastError().text();
        db.rollback();
        return;
    }



    db.commit();

    updateIds(false);
    currentIndex--;
}

bool DatabaseConnector::updateCard(const RepairCard &card)
{
    QString year = card.year == "20" || card.year.trimmed().isEmpty() ? "NULL" : card.year;
    QString month = card.month.trimmed() == "" ? "NULL" : card.month;

    db.transaction();
    QSqlQuery query;
    auto queryString = QString("update repair_cards"
                               " set repairer_id=%1,product_id=%2,client_id=%3,receive_date='%4',ready_date='%5',return='%6',state_id=%7,complaints='%8',reason='%9',note='%10',bar_code='%11',"
                               " cost_for_client=%12,cost_repair=%13,receive_date2='%14',sendDate='%15', createYear=%17, createMonth=%18"
                               " where id=%19")
            .arg(card.repairerId).arg(card.product.id).arg(card.clientId)
            .arg(card.receiveFromClientDate.toString("dd.MM.yyyy")).arg(card.readyDate.toString("dd.MM.yyyy")).arg(card.returnDate.toString("dd.MM.yyyy"))
            .arg(card.stateId)
            .arg(card.complaints).arg(card.reason).arg(card.note).arg(card.barCode)
            .arg(card.costForClient).arg(card.costRepair)
            .arg(card.receiveFromFactoryDate.toString("dd.MM.yyyy")).arg(card.sendDate.toString("dd.MM.yyyy"))
            .arg(year).arg(month)
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
    card.id = query.value("id").toInt();
    card.repairerId = query.value("repairer_id").toInt();
    card.clientId = query.value("client_id").toInt();
    card.receiveFromClientDate = QDate::fromString(query.value("receive_date").toString(),"dd.MM.yyyy");
    card.readyDate= QDate::fromString(query.value("ready_date").toString(),"dd.MM.yyyy");
    card.returnDate= QDate::fromString(query.value("return").toString(),"dd.MM.yyyy");
    card.stateId = query.value("state_id").toInt();
    card.complaints = query.value("complaints").toString();
    card.reason = query.value("reason").toString();
    card.note = query.value("note").toString();
    card.barCode = query.value("bar_code").toString();
    card.costForClient = query.value("cost_for_client").toInt();
    card.costRepair = query.value("cost_repair").toInt();
    card.receiveFromFactoryDate = QDate::fromString(query.value("receive_date2").toString(), "dd.MM.yyyy");
    card.sendDate = QDate::fromString(query.value("sendDate").toString(), "dd.MM.yyyy");
    card.year = query.value("createYear").toString();
    card.month = query.value("createMonth").toString();
    card.client.phone = query.value("phone").toString();
    card.client.person = query.value("person").toString();
    card.client.address = query.value("address").toString();
    card.state = query.value("states.name").toString();
    card.repairer = query.value("repairers.name").toString();
    card.client.name = query.value("clients.name").toString();
    card.product.id = query.value("products.id").toInt();
    card.product.code = query.value("products.code").toString();
    card.product.name = query.value("products.name").toString();
    card.product.isOwen = query.value("isOwen").toInt();
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

void DatabaseConnector::runFile(const QString &fileName)
{
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly))
        return;
    QStringList scriptQueries = QTextStream(&scriptFile).readAll().split(';');

    foreach (QString queryTxt, scriptQueries)
    {
        QSqlQuery query;
        if (queryTxt.trimmed().isEmpty())
            continue;

        auto f = query.exec(queryTxt);
        lastError = query.lastError().text();
        query.finish();
        query.clear();
    }
}

QString DatabaseConnector::getLastError() const
{
    return lastError;
}

void DatabaseConnector::createTables()
{
    runFile(":/sql/create_tables.sql");
    runFile(":/sql/insert_states.sql");
}

void DatabaseConnector::convert()
{
    QSqlQuery query;
    query.exec("select number from convert where id=0");
    query.first();
    auto dbVersion = query.value(0).toInt();

    QDirIterator it(":/convert", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        auto fileName = it.next();
        auto sl = fileName.lastIndexOf('/');
        auto v = fileName.mid(sl + 1, fileName.count() - 5 - sl);
        if (v.toInt() <= dbVersion)
            continue;
        runFile(fileName);
        query.exec(QString("update convert set number=%1 where id=0").arg(v));
    }
}

QSqlTableModel *DatabaseConnector::getTableModel(const QString &table)
{
    QSqlTableModel * model = new QSqlTableModel(nullptr,db);
    model->setTable(table);
    model->select();
    while(model->canFetchMore())
        model->fetchMore();
    return model;
}

DatabaseConnector *DatabaseConnector::getInstance()
{
    static DatabaseConnector* instance = nullptr;

    if (instance == nullptr)
        instance = new DatabaseConnector();

    return instance;
}

void DatabaseConnector::setCurrentIndex(int value)
{
    currentIndex = ids.indexOf(value);
}

RepairCard DatabaseConnector::getCardById(int id)
{
    RepairCard card;
    QSqlQuery query;
    query.exec(QString("SELECT c.*, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name, p.*  FROM repair_cards c left join products p on c.product_id=p.id left join clients cl on c.client_id=cl.id left join states st on c.state_id=st.id left join repairers rep on c.repairer_id=rep.id where c.id=%1").arg(id));
    if (query.first())
        fillCard(card, query);
    return card;
}

RepairCard DatabaseConnector::getCardByIndex(int index)
{
    index = index - 1;

    if (index >= ids.count())
        return getLastCard();
    else if (index < 0)
        return getFirstCard();

    currentIndex = index;
    return getCardById(ids[index]);
}

QVector<RepairCard> DatabaseConnector::getCardsByProductIdAndBarcode(int id, const QString& barcode)
{
    QSqlQuery query;
    query.exec(QString("SELECT * FROM repair_cards WHERE product_id=%1 AND bar_code='%2' ORDER BY receive_date").arg(id).arg(barcode));
    QVector<RepairCard> cards;

    while (query.next())
    {
        RepairCard card;
        fillCard(card, query);
        cards.push_back(card);
    }
    return cards;
}

QVector<RepairCard> DatabaseConnector::getRepairCardsByProductNameOrCode(const QString &nameOrCode, bool issuedProducts)
{
    QSqlQuery query;
    QString txt(QString("SELECT c.*, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name, p.* "
                           " FROM repair_cards c "
                           "left join products p on c.product_id=p.id "
                           "left join clients cl on c.client_id=cl.id"
                           " left join states st on c.state_id=st.id "
                           "left join repairers rep "
                           "on c.repairer_id=rep.id "
                           "where (p.name like '\%%1%' "
                           "or p.code like '\%%1%' "
                           "or c.bar_code like '\%%1%'"
                           "or cl.name like '\%%1%') and c.return %2 ''").arg(nameOrCode, issuedProducts ? "<>" : "=" ));
    query.exec(txt);

    QVector<RepairCard> cards;

    while (query.next())
    {
        RepairCard card;
        fillCard(card, query);
        card.name = QString("%1 %2 %4 %5")
                .arg(card.receiveFromClientDate.toString("dd.MM.yyyy"))
                .arg(card.product.name).arg(card.barCode).arg(card.client.name);
        card.currentIndex = ids.indexOf(card.id) + 1;
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
    {
        lastError = query.lastError().text();
        return -1;
    }

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
    auto queryString = QString("insert into products('name','code', 'isOwen') values ('%1', '%2', %3)")
            .arg(product.name)
            .arg(product.code)
            .arg(product.isOwen);

    if (!query.exec(queryString))
        return -1;

    return query.lastInsertId().toInt();
}

bool DatabaseConnector::updateProduct(const Product &product)
{
    QSqlQuery query;
    auto queryString = QString("update products set name='%1', code='%2', isOwen=%3 where id=%4")
            .arg(product.name).arg(product.code).arg(product.isOwen).arg(product.id);

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
    auto q = QString("SELECT c.*, cl.phone, cl.person, cl.address, st.name, rep.name, cl.name, p.* FROM repair_cards c left join products p on c.product_id=p.id left join clients cl on c.client_id=cl.id left join states st on c.state_id=st.id left join repairers rep on c.repairer_id=rep.id WHERE"
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

QVector<RepairCard> DatabaseConnector::getSendedCards()
{
    QSqlQuery query;
    auto q = QString("select c.*, p.* FROM repair_cards c left join products p on c.product_id = p.id where c.sendDate != \"\" and c.receive_date2 = \"\"");
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
