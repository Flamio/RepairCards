#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QSqlDatabase>
#include <QHash>
#include <QVector>
#include "handbook.h"
#include "client.h"
#include "repaircard.h"
#include "cardmethod.h"
#include "product.h"

class DatabaseConnector
{
public:
    DatabaseConnector();
    ~DatabaseConnector();

    bool open();

    QVector<Handbook*> getHandbook(const QString& handbookName);
    QHash<int,Client> getClients();

    RepairCard getLastCard();
    Handbook getProductByCode(const QString& code);
    bool addCard(const RepairCard& card);
    bool addMethods(const QVector<CardMethod> cardMethods);
    QVector<CardMethod> getMethods(int cardId);
    RepairCard getPreviousCard();
    RepairCard getNextCard();
    void deleteCard(int id);
    bool updateCard(const RepairCard& card);
    RepairCard getCardById(int id);
    int addHandbook(const Handbook& handbook, const QString& tableName);
    bool updateHandbook(const Handbook& handbook, const QString& tableName);
    void deleteHandbook(int id, const QString& tableName);

    int addProduct(const Product& product);
    bool updateProduct(const Product& product);
    void deleteProduct(int id);

    int getEntries(int id, const QString& column, const QString& table);
    QVector<Handbook*> getProducts();

private:
    QSqlDatabase db;
    QVector<int> ids;
    int currentIndex = 0;
    void fillCard(RepairCard& card, QSqlQuery& query);
    bool updateIds(bool currentChange=true);
};

#endif // DATABASECONNECTOR_H
