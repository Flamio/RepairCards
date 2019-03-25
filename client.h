#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include "handbook.h"

struct Client : Handbook
{
    QString phone;
    QString person;
    QString address;
};

#endif // CLIENT_H
