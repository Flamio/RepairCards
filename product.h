#ifndef PRODUCT_H
#define PRODUCT_H

#include "handbook.h"

struct Product : Handbook
{
    QString code;
    bool isOwen = false;
};

#endif // PRODUCT_H
