#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

struct Client
{
    int id = 0;
    QString name;
    QString phone;
    QString contact;
    QString address;
};

#endif // CLIENT_H
