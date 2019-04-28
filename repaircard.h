#ifndef REPAIRCARD_H
#define REPAIRCARD_H

#include <QDate>
#include "client.h"
#include "product.h"

struct RepairCard
{
    int id = 0;
    int repairerId = 1;
    int clientId = 0;
    int stateId = 0;
    int costForClient = 0;
    int costRepair = 0;
    QDate receiveFromClientDate = QDate::currentDate();
    QDate readyDate;
    QDate returnDate;
    QDate receiveFromFactoryDate;
    QDate sendDate;
    QString complaints = "";
    QString reason = "";
    QString note = "";
    QString barCode = "";
    Client client;
    Product product;
    QString state;
    QString repairer;
    int currentIndex = 0;
    int allIndexes = 0;
    QString year;
    QString month;
};

#endif // REPAIRCARD_H
