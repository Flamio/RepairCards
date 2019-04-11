#ifndef PRINTERFACTORY_H
#define PRINTERFACTORY_H

#include <QObject>
#include "printer.h"
#include "printtype.h"

class PrinterFactory : public QObject
{
    Q_OBJECT
public:
    explicit PrinterFactory(DatabaseConnector* dbconnector, QObject *parent = nullptr);
    Printer* build(PrintType::PrintType, bool);

signals:

public slots:

private:
    Printer* printer = nullptr;
    DatabaseConnector* dbconnector = nullptr;
};

#endif // PRINTERFACTORY_H
