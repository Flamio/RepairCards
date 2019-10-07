#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include "databaseconnector.h"

class Printer : public QObject
{
    Q_OBJECT
public:
    explicit Printer(QObject *parent = nullptr);
    virtual ~Printer();

    void setDbConnector(DatabaseConnector *value);

    void print(int id);



signals:

public slots:

protected:
    virtual void drawContent(int id, QPainter&) = 0;

    DatabaseConnector *getDbConnector() const;
    QPrinter *getPrinter() const;

private:
    QPrinter* printer = nullptr;
    QPrintDialog* printDialog = nullptr;
    DatabaseConnector* dbConnector;
    void free();
};

#endif // PRINTER_H
