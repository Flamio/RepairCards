#ifndef REPAIRCARDPRINTER_H
#define REPAIRCARDPRINTER_H

#include "printer.h"

class RepairCardPrinter : public Printer
{
public:
    RepairCardPrinter(QObject *parent = nullptr);

protected:
    virtual void drawContent(int id, QPainter&);
};

#endif // REPAIRCARDPRINTER_H
