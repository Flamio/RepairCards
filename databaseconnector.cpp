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
    query.exec("SELECT id, name FROM "+handbookName);
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

DatabaseConnector::DatabaseConnector()
{

}

DatabaseConnector::~DatabaseConnector()
{
    if (db.isOpen())
        db.close();
}
