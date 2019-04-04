#include "printer.h"

Printer::Printer(QObject *parent) : QObject(parent)
{
}

Printer::~Printer()
{
    free();
}

DatabaseConnector *Printer::getDbConnector() const
{
    return dbConnector;
}

void Printer::setDbConnector(DatabaseConnector *value)
{
    dbConnector = value;
}

void Printer::free()
{
    if (printer != nullptr)
        delete printer;
    if (printDialog != nullptr)
        delete printDialog;
}

void Printer::print(int id)
{
    free();

    printer = new QPrinter();
    printDialog = new QPrintDialog(printer);

    printDialog->setWindowTitle("Печать");

    if (printDialog->exec() != QDialog::Accepted)
        return;

    QPainter painter;
    painter.begin(printer);

    drawContent(id,painter);

    painter.end();
}
