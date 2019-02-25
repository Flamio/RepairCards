#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QSqlDatabase>
#include <QVector>
#include "handbook.h"

class DatabaseConnector
{
public:
    DatabaseConnector();
    ~DatabaseConnector();

    bool open();

    QVector<Handbook> getHandbook(const QString& handbookName);

private:
    QSqlDatabase db;
};

#endif // DATABASECONNECTOR_H
