#ifndef INVOICEPRINTER_H
#define INVOICEPRINTER_H

#include "printer.h"

class InvoicePrinter : public Printer
{
public:
    InvoicePrinter(QObject* parent = nullptr);

protected:
    virtual void drawContent(int id, QPainter&);
};

#endif // INVOICEPRINTER_H
