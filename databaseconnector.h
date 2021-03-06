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
    Product getLastProduct();
    Product getProductByCode(const QString& code);
    QVector<Product> getProductsByName(const QString& name, bool isNotOwenOnly = false);
    int getProductCountWithTheSameCode(const QString& code, int id);
    bool addCard(const RepairCard& card);
    bool addMethods(const QVector<CardMethod> cardMethods);
    QVector<CardMethod> getMethods(int cardId);
    RepairCard getPreviousCard();
    RepairCard getNextCard();
    RepairCard getFirstCard();
    void deleteCard(int id);
    bool updateCard(const RepairCard& card);
    RepairCard getCardById(int id);
    RepairCard getCardByIndex(int index);
    QVector<RepairCard> getCardsByProductIdAndBarcode(int id, const QString& barcode);
    QVector<RepairCard> getRepairCardsByProductNameOrCode(const QString& nameOrCode, bool issuedProducts);
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
    QVector<RepairCard> getSendedCards();

    QString getLastError() const;

    void createTables();
    void convert();

    QSqlTableModel* getTableModel(const QString& table);

    static DatabaseConnector* getInstance();
    void setCurrentIndex(int value);

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
