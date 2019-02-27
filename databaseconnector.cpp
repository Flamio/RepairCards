#include "databaseconnector.h"
#include <QSqlQuery>
#include <QVariant>

bool DatabaseConnector::open()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("repair_cards.db3");
    return db.open();
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


DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
    if (db.isOpen())
        db.close();
}
