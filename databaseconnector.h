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
    RepairCard getPreviousCard();
    RepairCard getNextCard();

private:
    QSqlDatabase db;
    QVector<int> ids;
    RepairCard getCardById(int id);
    int currentIndex = 0;
    void fillCard(RepairCard& card, QSqlQuery& query);
    bool updateIds();
};

#endif // DATABASECONNECTOR_H
