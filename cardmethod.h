#ifndef CARDMETHOD_H
#define CARDMETHOD_H
#include <QString>

struct CardMethod
{
    int cardId = 0;
    int methodId = 0;
    QString description = "";
    QString methodName = "";
};

#endif // CARDMETHOD_H
