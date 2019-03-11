#ifndef REPAIRCARD_H
#define REPAIRCARD_H

#include <QDate>
#include "client.h"

struct RepairCard
{
    int id = 0;
    int repairerId = 0;
    int productId = 0;
    int clientId = 0;
    int stateId = 0;
    int costForClient = 0;
    int costRepair = 0;
    QDate receiveDate = QDate::currentDate();
    QDate readyDate = QDate::currentDate();
    QDate returnDate = QDate::currentDate();
    QString complaints = "";
    QString reason = "";
    QString note = "";
    QString productName = "";
    QString barCode = "";
    Client client;
    QString state;
    QString repairer;
    int currentIndex = 0;
    int allIndexes = 0;
};

#endif // REPAIRCARD_H
