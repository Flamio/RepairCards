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
#include <QSqlTableModel>

class DatabaseConnector
{
public:
    DatabaseConnector();
    ~DatabaseConnector();

    bool open();

    QVector<Handbook*> getHandbook(const QString& handbookName);
    QVector<Handbook*> getClients();

    RepairCard getLastCard();
    Product getProductByCode(const QString& code);
    bool addCard(const RepairCard& card);
    bool addMethods(const QVector<CardMethod> cardMethods);
    QVector<CardMethod> getMethods(int cardId);
    RepairCard getPreviousCard();
    RepairCard getNextCard();
    void deleteCard(int id);
    bool updateCard(const RepairCard& card);
    RepairCard getCardById(int id);
    QVector<RepairCard> getCardsByProductId(int id);
    int addHandbook(const Handbook& handbook, const QString& tableName);
    bool updateHandbook(const Handbook& handbook, const QString& tableName);
    void deleteHandbook(int id, const QString& tableName);

    int addProduct(const Product& product);
    bool updateProduct(const Product& product);
    void deleteProduct(int id);

    int addClient(const Client& product);
    bool updateClient(const Client& product);
    void deleteClient(int id);

    int getEntries(int id, const QString& column, const QString& table);
    QVector<Handbook*> getProducts();
    QVector<RepairCard> getCardsByDateAndClient(QDate,int clientId);

    QString getLastError() const;

    void createTables();
    void convert();

    QSqlTableModel* getTableModel(const QString& table);



private:
    QSqlDatabase db;
    QVector<int> ids;
    int currentIndex = 0;
    void fillCard(RepairCard& card, QSqlQuery& query);
    bool updateIds(bool currentChange=true);
    void runFile(const QString& fileName);

    QString lastError;
    RepairCard bufCard;
};

#endif // DATABASECONNECTOR_H
