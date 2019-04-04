#include "repaircardprinter.h"

RepairCardPrinter::RepairCardPrinter(QObject *parent) : Printer(parent)
{

}

void RepairCardPrinter::drawContent(int id, QPainter &painter)
{
    painter.drawText(0, 0, 500, 500, Qt::AlignLeft|Qt::AlignTop, "Test");
}
