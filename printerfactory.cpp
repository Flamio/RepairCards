#include "printerfactory.h"
#include "repaircardprinter.h"
#include "invoiceprinter.h"
#include "paidcardprinter.h"

PrinterFactory::PrinterFactory(DatabaseConnector* dbconnector,QObject *parent) : QObject(parent)
{
    this->dbconnector = dbconnector;
}

Printer *PrinterFactory::build(PrintType::PrintType type, bool isWaranty)
{
    if (printer != nullptr)
        delete printer;

    switch (type) {
    case PrintType::RepairCard:
        printer = isWaranty ? nullptr : new PaidCardPrinter(this);
        break;
    case PrintType::Invoice:
        printer = new InvoicePrinter(this);
        break;

    default:
        printer = nullptr;
    }

    if (printer == nullptr)
        return printer;

    printer->setDbConnector(dbconnector);
    return printer;
}
