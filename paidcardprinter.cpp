#include "paidcardprinter.h"

PaidCardPrinter::PaidCardPrinter(QObject* parent) : Printer(parent)
{

}

void PaidCardPrinter::drawContent(int id, QPainter &painter)
{
    auto dbConnector = getDbConnector();
    auto card = dbConnector->getCardById(id);

    painter.setWindow(0, 0, 4958, 7016);
    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(95);
    painter.setFont(font);

    auto metrics = new QFontMetrics(font);

    painter.drawText(620,700, QString("Карточка платного ремонта №%1").arg(card.id));
    auto s = QString("Дата приема %1").arg(card.receiveFromClientDate.toString("dd.MM.yyyy"));
    painter.drawText(4958-620-metrics->width(s),700, s);

    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    painter.drawLine(620,774, 4338, 774);

}
