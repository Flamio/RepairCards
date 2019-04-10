#ifndef PAIDCARDPRINTER_H
#define PAIDCARDPRINTER_H

#include "printer.h"

class PaidCardPrinter : public Printer
{
public:
    PaidCardPrinter(QObject* parent = nullptr);

protected:
    virtual void drawContent(int id, QPainter&);
};

#endif // PAIDCARDPRINTER_H
