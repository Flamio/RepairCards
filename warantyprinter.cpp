#include "warantyprinter.h"

WarantyPrinter::WarantyPrinter(QObject *parent) : Printer(parent)
{

}

void WarantyPrinter::drawContent(int id, QPainter &painter)
{
    auto dbConnector = getDbConnector();
    auto card = dbConnector->getCardById(id);

    painter.setWindow(0,0,826,1169);

    QFont header1Font;
    header1Font.setFamily("Arial");
    header1Font.setPixelSize(30);
    header1Font.setItalic(true);
    painter.setFont(header1Font);

    QFont header2Font;
    header2Font.setFamily("Arial");
    header2Font.setPixelSize(12);
    header2Font.setBold(true);
    header2Font.setItalic(true);

    QFont header3Font;
    header3Font.setFamily("Arial");
    header3Font.setPixelSize(16);
    header3Font.setBold(true);

    QFont boldTextFont;
    boldTextFont.setFamily("Arial");
    boldTextFont.setPixelSize(10);
    boldTextFont.setBold(true);

    QFont textFont;
    textFont.setFamily("Arial");
    textFont.setPixelSize(10);


    painter.drawText(100,128, "ООО \"Системы автоматизации\"");
    painter.drawLine(100, 142, 710, 142);

    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::gray);
    painter.setPen(pen);

    painter.drawLine(100, 145, 710, 145);
    painter.drawLine(100, 182, 710, 182);

    pen.setColor(Qt::black);
    painter.setPen(pen);

    painter.setFont(header2Font);
    painter.drawText(100,150, 610, 30, Qt::AlignLeft | Qt::TextWordWrap, "Россия, 426077, Удмуртская республика, г.Ижевск, ул. Удмуртская, 161А"
                                                                         " тел. 3412/52-92-98, 52-93-39 факс 3412/52-92-75, 52-92-98"
                                                                         " ИНН 1841028197 КПП 18410100");

    painter.setFont(header3Font);
    painter.drawText(195, 265, "Перечень выполненных работ по ремонту изделия");



}
