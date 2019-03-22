#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include "handbook.h"

struct Client : Handbook
{
    QString phone;
    QString contact;
    QString address;
};

#endif // CLIENT_H
