#include "invoiceprinter.h"
#include <QRect>
#include <QtMath>

InvoicePrinter::InvoicePrinter(QObject* parent) : Printer(parent)
{

}

int InvoicePrinter::calcCellHeight(const QString& text, int cellWidth)
{
    return (int)qCeil((float)fm->width(text) / (float)cellWidth * fm->height()*1.3f);
}

void InvoicePrinter::drawRow(QHash<QString,QPair<QString, short>> rowData)
{

    int height = 0;
    height = calcCellHeight(rowData["note"].first, noteWidth);

    if (height == 0)
        height = fm->height();

    height+=150;

    QRect r1(345, y, nameWidth, height);
    QRect r2(345 + nameWidth, y, barcodeWidth, height);
    QRect r3(345 + nameWidth + barcodeWidth, y, noteWidth, height);
    painter->drawRect(r1);
    painter->drawRect(r2);
    painter->drawRect(r3);
    r1.setX(r1.x()+10);
    r1.setY(r1.y()+10);
    r1.setWidth(r1.width() -10);
    r2.setY(r2.y()+10);
    r2.setX(r2.x()+10);
    r2.setWidth(r2.width() -10);
    r3.setX(r3.x()+10);
    r3.setY(r3.y()+10);
    r3.setWidth(r3.width() -10);

    painter->drawText(r1, rowData["name"].second, rowData["name"].first);
    painter->drawText(r2, rowData["barcode"].second, rowData["barcode"].first);
    painter->drawText(r3, rowData["note"].second, rowData["note"].first);

    y+=height;
}

int InvoicePrinter::drawListWithTable(const QVector<RepairCard>& cards)
{
    int index = -1;
    QFont font;
    font.setFamily("Arial");
    painter->setFont(font);

    font.setPixelSize(70);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(3350, 204, 900, 75,Qt::AlignLeft|Qt::AlignTop, "Контактные телефоны:");
    font.setBold(false);
    painter->setFont(font);
    painter->drawText(3360, 290, 800, 246, Qt::AlignRight|Qt::AlignTop, "8(3412)52-92-75\n"
                                                                        "52-92-98\n"
                                                                        "52-93-39");
    font.setBold(true);
    font.setPixelSize(90);
    painter->setFont(font);

    painter->drawText(735, 860,  "Накладная на прием изделий в ремонт");
    painter->drawText(2964, 860,  cards.first().receiveFromClientDate.toString("dd.MM.yyyy"));

    font.setPixelSize(80);
    painter->setFont(font);
    fm= new QFontMetrics(font);
    y = 1135;
    painter->drawText(QRect(345, y,348,fm->height()),  "От кого:");
    auto client = QString("%1 %2").arg(cards.first().client.name).arg(cards.first().client.address);
    auto height = (int)qCeil(fm->width(client) / 3324.0f) * fm->height();
    painter->drawText(QRect(735, y, 3324, height), Qt::TextWordWrap, client);
    y+=height+fm->height();
    painter->drawText(QRect(345, y, 348, fm->height()),  "Кому:");
    auto firm = "ООО \"Системы автоматизации\" Удмуртская республика, г.Ижевск, ул. Удмуртская, 161А";
    height = (int)qCeil(fm->width(firm) / 3324.0f) * fm->height();
    painter->drawText(QRect(735, y, 3324, height), Qt::TextWordWrap, firm);
    y+=height+fm->height()*2;

    font.setBold(false);
    painter->setFont(font);
    delete fm;
    fm = new QFontMetrics(font);

    for (auto c : cards)
        if (fm->width(c.product.name) > nameWidth)
            nameWidth = fm->width(c.product.name);

    if (nameWidth < fm->width("Наименование"))
        nameWidth = fm->width("Наименование");

    nameWidth+=50;

    noteWidth = tableWidth - barcodeWidth - nameWidth;

    QPen pen;
    pen.setWidth(5);
    painter->setPen(pen);

    drawRow(QHash<QString,QPair<QString, short>>{{"name", qMakePair(QString("Наименование"), Qt::AlignCenter)},
                                                 {"barcode", qMakePair(QString("Штрихкод"), Qt::AlignCenter)},
                                                 {"note", qMakePair(QString("Примечание"), Qt::AlignCenter)},
            });

    auto cardsCount = cards.count();

    for (int i = 0; i < cards.count(); i++)
    {
        if (y + calcCellHeight(cards[i].note, noteWidth) > 5000)
        {
            index = i;
            cardsCount = i;
            break;
        }

        drawRow(QHash<QString,QPair<QString, short>>{{"name", qMakePair(cards[i].product.name, Qt::AlignLeft | Qt::TextWordWrap)},
                                                     {"barcode", qMakePair(cards[i].barCode, Qt::AlignLeft | Qt::TextWordWrap)},
                                                     {"note", qMakePair(cards[i].note, Qt::AlignLeft | Qt::TextWordWrap)}
                });


    }

    y+=fm->height()*2;
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QRect(345, y,400,fm->height()),  QString("Итого: %1").arg(cardsCount));
    y+=fm->height()*2;
    painter->drawText(QRect(1692, y,400,fm->height()),  "Принял:");
    painter->drawLine(2045, y + fm->height()+10, 4033, y + fm->height()+10);
    y+=fm->height()*4;
    painter->drawText(QRect(2700, y,400,fm->height()),  "Дата:");
    painter->drawLine(2950, y + fm->height()+10, 4033, y + fm->height()+10);
    painter->drawText(QRect(500, y, 400,fm->height()),  "Получил:");
    painter->drawLine(900, y + fm->height()+10, 2550, y + fm->height()+10);

    delete fm;
    return index;
}

void InvoicePrinter::drawContent(int id, QPainter &painter)
{
    this->painter = &painter;
    painter.setWindow(0,0,4397,6263);
    auto db = getDbConnector();
    auto card = db->getCardById(id);
    auto cards = db->getCardsByDateAndClient(card.receiveFromClientDate, card.clientId);

    int index = 0;

    while (index != -1)
    {
        if (index != 0)
        {
            cards = cards.mid(index, cards.length() - index);
            if (cards.count() != 0)
                getPrinter()->newPage();
            else
                break;

        }
        index = drawListWithTable(cards);

    }
}
