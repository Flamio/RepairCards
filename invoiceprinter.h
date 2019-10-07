#ifndef INVOICEPRINTER_H
#define INVOICEPRINTER_H

#include "printer.h"

class InvoicePrinter : public Printer
{
public:
    InvoicePrinter(QObject* parent = nullptr);

protected:
    virtual void drawContent(int id, QPainter&);
private:
    const int tableWidth = 3714;
    const int barcodeWidth = 1050;
    int noteWidth = 0;
    int nameWidth = 0;
    int y = 0;

    void drawRow(QHash<QString,QPair<QString, short>> rowData);

    QFontMetrics* fm = nullptr;
    QPainter* painter = nullptr;
    int drawListWithTable(const QVector<RepairCard>& cards);
    int calcCellHeight(const QString& text, int cellWidth);
};

#endif // INVOICEPRINTER_H
