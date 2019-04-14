#include "warantyprinter.h"
#include <QtMath>

WarantyPrinter::WarantyPrinter(QObject *parent) : Printer(parent)
{
    boldTextFont.setFamily("Arial");
    boldTextFont.setPixelSize(15);
    boldTextFont.setBold(true);

    textFont.setFamily("Arial");
    textFont.setPixelSize(15);

    fm = new QFontMetrics(textFont);
    fmBold = new QFontMetrics(boldTextFont);
}

WarantyPrinter::~WarantyPrinter()
{
    delete fm;
    delete fmBold;
}

void WarantyPrinter::drawContent(int id, QPainter &painter)
{
    this->painter = &painter;
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

    int y = 320;

    drawText("Изделие", QString("%1 зав.№ %2").arg(card.productName).arg(card.barCode), y);
    drawText("было принято в ремонт", card.receiveFromClientDate.toString("dd.MM.yyyy"), y);
    drawText("от", QString("%1 %2").arg(card.client.name).arg(card.client.address), y);
    drawText("с жалобой", card.complaints, y);
    drawText("Причина отказа", card.reason, y);
    drawText("Перечень работ:", "", y);

    auto methods = dbConnector->getMethods(card.id);

    for (auto method : methods)
    {
        painter.drawText(100,y, method.methodName);
        y+=20;
    }

    y+=20;

    drawText("Исполнитель:", card.repairer, y);

    painter.drawText(100, y, QDate::currentDate().toString("dd.MM.yyyy"));
}

void WarantyPrinter::drawText(QString bold, QString text, int &y)
{
    auto textBegin = fmBold->width(bold) + 110;
    painter->setFont(boldTextFont);
    painter->drawText(100, y, fmBold->width(bold) , fmBold->height(), Qt::AlignLeft, bold);
    painter->setFont(textFont);
    auto height = (int)qCeil(fm->width(text) / (float)(610 - textBegin+110)) * fm->height();
    painter->drawText(textBegin, y, 610 - textBegin+110,  40 +height, Qt::TextWordWrap, text);
    y+=40 + height;
}
