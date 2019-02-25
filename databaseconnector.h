#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QSqlDatabase>
#include <QHash>
#include <QVector>
#include "handbook.h"
#include "client.h"

class DatabaseConnector
{
public:
    DatabaseConnector();
    ~DatabaseConnector();

    bool open();

    QVector<Handbook> getHandbook(const QString& handbookName);
    QHash<int,Client> getClients();

private:
    QSqlDatabase db;
};

#endif // DATABASECONNECTOR_H
