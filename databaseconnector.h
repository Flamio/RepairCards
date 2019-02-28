#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QSqlDatabase>
#include <QHash>
#include <QVector>
#include "handbook.h"
#include "client.h"
#include "repaircard.h"
#include "cardmethod.h"

class DatabaseConnector
{
public:
    DatabaseConnector();
    ~DatabaseConnector();

    bool open();

    QVector<Handbook> getHandbook(const QString& handbookName);
    QHash<int,Client> getClients();

    RepairCard getLastCard();
    Handbook getProductByCode(const QString& code);
    bool addCard(const RepairCard& card);
    bool addMethods(const QVector<CardMethod> cardMethods);
    QVector<CardMethod> getMethods(int cardId);

private:
    QSqlDatabase db;
};

#endif // DATABASECONNECTOR_H
