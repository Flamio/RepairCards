#ifndef WARANTYPRINTER_H
#define WARANTYPRINTER_H

#include "printer.h"

class WarantyPrinter : public Printer
{
    Q_OBJECT
public:
    explicit WarantyPrinter(QObject *parent = 0);

signals:

public slots:

protected:
    virtual void drawContent(int id, QPainter&);
};

#endif // WARANTYPRINTER_H
