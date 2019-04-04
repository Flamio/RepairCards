#include "invoiceprinter.h"
#include <QRect>
#include <QtMath>

InvoicePrinter::InvoicePrinter(QObject* parent) : Printer(parent)
{

}

void InvoicePrinter::drawContent(int id, QPainter &painter)
{
    auto db = getDbConnector();
    auto card = db->getCardById(id);
    auto cards = db->getCardsByDateAndClient(card.receiveFromClientDate, card.clientId);

    QFont font;
    font.setFamily("Arial");
    painter.setFont(font);

    font.setPixelSize(11);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(600, 80,  "Контактные телефоны:");
    font.setBold(false);
    painter.setFont(font);
    painter.drawText(600, 90, 125, 40, Qt::AlignRight|Qt::AlignTop, "8(3412)52-92-75\n"
                                                                    "52-92-98\n"
                                                                    "52-93-39");
    font.setBold(true);
    font.setPixelSize(15);
    painter.setFont(font);

    painter.drawText(166, 180,  "Накладная на прием изделий в ремонт");
    painter.drawText(530, 180,  card.receiveFromClientDate.toString("dd.MM.yyyy"));

    QFontMetrics* fm= new QFontMetrics(font);
    auto y = 230;
    painter.drawText(QRect(100, y,60,fm->height()),  "От кого:");
    auto client = QString("%1 %2").arg(card.client.name).arg(card.client.address);
    auto height = (int)qCeil(fm->width(client) / 560.0f) * fm->height();
    painter.drawText(QRect(166, y, 560, height), Qt::TextWordWrap, client);
    y+=height+fm->height();
    painter.drawText(QRect(100, y, 60, fm->height()),  "Кому:");
    auto firm = "ООО \"Системы автоматизации\" Удмуртская республика, г.Ижевск, ул. Удмуртская, 161А";
    height = (int)qCeil(fm->width(firm) / 560.0f) * fm->height();
    painter.drawText(QRect(166, y, 560, height), Qt::TextWordWrap, firm);
    y+=height+fm->height();

    const int tableWidth = 617;
    const int barcodeWidth = 170;
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

    nameWidth+=5;

    int noteWidth = tableWidth - barcodeWidth - nameWidth;

    for (int i = -1; i < cards.count(); i++)
    {
        const int heightP = 10;
        QRect r1(100, y, nameWidth, fm->height()+heightP);
        QRect r2(100 + nameWidth, y, barcodeWidth, fm->height()+heightP);
        QRect r3(100 + nameWidth + barcodeWidth, y, noteWidth, fm->height()+heightP);
        painter.drawRect(r1);
        painter.drawRect(r2);
        painter.drawRect(r3);
        r1.setX(r1.x()+2);
        r1.setWidth(r1.width() -2);
        r2.setX(r2.x()+2);
        r2.setWidth(r2.width() -2);
        r3.setX(r3.x()+2);
        r3.setWidth(r3.width() -2);

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
    painter.drawText(QRect(100, y,60,fm->height()),  QString("Итого: %1").arg(cards.count()));
    y+=fm->height()*2;
    painter.drawText(QRect(140, y,60,fm->height()),  "Сдал:");
    painter.drawText(QRect(441, y,60,fm->height()),  "Принял:");

    painter.drawLine(200, y + fm->height()*1.5, 280, y + fm->height()*1.5);
    painter.drawLine(300, y + fm->height()*1.5, 400, y + fm->height()*1.5);
    painter.drawLine(510, y + fm->height()*1.5, 590, y + fm->height()*1.5);
    painter.drawLine(610, y + fm->height()*1.5, 710, y + fm->height()*1.5);
}
