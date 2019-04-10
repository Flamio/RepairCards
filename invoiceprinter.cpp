#include "invoiceprinter.h"
#include <QRect>
#include <QtMath>

InvoicePrinter::InvoicePrinter(QObject* parent) : Printer(parent)
{

}

void InvoicePrinter::drawContent(int id, QPainter &painter)
{
    painter.setWindow(0,0,4397,6263);
    auto db = getDbConnector();
    auto card = db->getCardById(id);
    auto cards = db->getCardsByDateAndClient(card.receiveFromClientDate, card.clientId);

    QFont font;
    font.setFamily("Arial");
    painter.setFont(font);

    font.setPixelSize(70);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(3350, 204, 900, 75,Qt::AlignLeft|Qt::AlignTop, "Контактные телефоны:");
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(3360, 290, 800, 246, Qt::AlignRight|Qt::AlignTop, "8(3412)52-92-75\n"
                                                                    "52-92-98\n"
                                                                    "52-93-39");
    font.setBold(true);
    font.setPixelSize(90);
    painter.setFont(font);

    painter.drawText(735, 860,  "Накладная на прием изделий в ремонт");
    painter.drawText(2964, 860,  card.receiveFromClientDate.toString("dd.MM.yyyy"));

    font.setPixelSize(80);
    painter.setFont(font);
    QFontMetrics* fm= new QFontMetrics(font);
    auto y = 1135;
    painter.drawText(QRect(345, y,348,fm->height()),  "От кого:");
    auto client = QString("%1 %2").arg(card.client.name).arg(card.client.address);
    auto height = (int)qCeil(fm->width(client) / 3324.0f) * fm->height();
    painter.drawText(QRect(735, y, 3324, height), Qt::TextWordWrap, client);
    y+=height+fm->height();
    painter.drawText(QRect(345, y, 348, fm->height()),  "Кому:");
    auto firm = "ООО \"Системы автоматизации\" Удмуртская республика, г.Ижевск, ул. Удмуртская, 161А";
    height = (int)qCeil(fm->width(firm) / 3324.0f) * fm->height();
    painter.drawText(QRect(735, y, 3324, height), Qt::TextWordWrap, firm);
    y+=height+fm->height()*2;

    const int tableWidth = 3714;
    const int barcodeWidth = 1050;
    auto nameWidth = 0;

    font.setBold(false);
    painter.setFont(font);
    delete fm;
    fm = new QFontMetrics(font);

    for (auto c : cards)
        if (fm->width(c.productName) > nameWidth)
            nameWidth = fm->width(c.productName);

    if (nameWidth < fm->width("Наименование"))
        nameWidth = fm->width("Наименование");

    nameWidth+=50;

    int noteWidth = tableWidth - barcodeWidth - nameWidth;

    for (int i = -1; i < cards.count(); i++)
    {
        QPen pen;
        pen.setWidth(5);
        painter.setPen(pen);
        const int heightP = 100;
        QRect r1(345, y, nameWidth, fm->height()+heightP);
        QRect r2(345 + nameWidth, y, barcodeWidth, fm->height()+heightP);
        QRect r3(345 + nameWidth + barcodeWidth, y, noteWidth, fm->height()+heightP);
        painter.drawRect(r1);
        painter.drawRect(r2);
        painter.drawRect(r3);
        r1.setX(r1.x()+10);
        r1.setWidth(r1.width() -10);
        r2.setX(r2.x()+10);
        r2.setWidth(r2.width() -10);
        r3.setX(r3.x()+10);
        r3.setWidth(r3.width() -10);

        if (i == -1)
        {
            painter.drawText(r1, Qt::AlignCenter, "Наименование");
            painter.drawText(r2, Qt::AlignCenter, "Штрихкод");
            painter.drawText(r3, Qt::AlignCenter, "Примечание");
        }
        else {
            painter.drawText(r1, Qt::AlignLeft | Qt::AlignVCenter, cards[i].productName);
            painter.drawText(r2, Qt::AlignLeft | Qt::AlignVCenter, cards[i].barCode);
            painter.drawText(r3, Qt::AlignLeft | Qt::AlignVCenter, cards[i].note);
        }
        y+=fm->height()+heightP;
    }

    y+=fm->height()*2;
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(QRect(345, y,400,fm->height()),  QString("Итого: %1").arg(cards.count()));
    y+=fm->height()*2;
    painter.drawText(QRect(1692, y,400,fm->height()),  "Сдал:");
     painter.drawLine(2039, y + fm->height()+10, 4033, y + fm->height()+10);
    y+=fm->height()*3;
    painter.drawText(QRect(1692, y,400,fm->height()),  "Принял:");
    painter.drawLine(2182, y + fm->height()+10, 4033, y + fm->height()+10);
}
