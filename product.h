#ifndef PRODUCT_H
#define PRODUCT_H

#include "handbook.h"

struct Product : Handbook
{
    QString code;
    bool isOwen = true;
};

#endif // PRODUCT_H
