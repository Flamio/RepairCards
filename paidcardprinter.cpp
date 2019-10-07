#include "paidcardprinter.h"
#include <QtMath>
#include "helper.h"

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
    pen.setWidth(20);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    painter.drawLine(620,774, 4338, 774);
    painter.drawLine(620,2320, 4338, 2320);

    pen.setColor(Qt::black);
    painter.setPen(pen);
    font.setPixelSize(85);
    painter.setFont(font);
    painter.drawText(620, 974, "Предприятие");
    painter.drawText(620,1111, "Адрес");
    painter.drawText(620,1248, "Изделие");
    painter.drawText(620,1385, "Год выпуска");
    painter.drawText(1425, 1385, "месяц");
    painter.drawText(2832, 1385, "Штрихкод");

    pen.setWidth(5);
    painter.setPen(pen);
    for (int i = 0, j=1842; i < 5; i++, j+=93)
        painter.drawLine(620, j, 4338, j);

    delete metrics;
    metrics = new QFontMetrics(font);
    painter.drawText(QRect(635, 1840 - metrics->height(), 3600, 465), Qt::TextWordWrap , card.complaints);

    painter.drawText(620, 2580, "Ф.И.О. сервис-инженера");

    for (int i = 0, j=2871; i < 5; i++, j+=93)
        painter.drawLine(620, j, 4338, j);

    painter.drawText(QRect(635, 2869 - metrics->height(), 3600, 465), Qt::TextWordWrap , card.reason);

    auto methods = dbConnector->getMethods(card.id);

    auto y = 3360;
    for (int i = -1; i < methods.count(); i++)
    {

        if (i == -1)
        {
            QRect rect(620, y, 1416, 174);
            QRect rect2(2036, y, 2300, 174);
            font.setBold(true);
            painter.setFont(font);
            painter.fillRect(rect, Qt::gray);
            painter.drawRect(rect);
            painter.fillRect(rect2, Qt::gray);
            painter.drawRect(rect2);
            painter.drawText(rect, Qt::AlignCenter, "Способ устранения");
            painter.drawText(rect2, Qt::AlignCenter, "Описание");
            y+=174;
            continue;
        }

        font.setBold(false);
        font.setPixelSize(80);
        delete metrics;
        metrics = new QFontMetrics(font);
        painter.setFont(font);

        auto height1 = (int)qCeil(metrics->width(methods[i].methodName) / 1416.0f) * metrics->height();
        auto height2 = (int)qCeil(metrics->width(methods[i].description) / 2300.0f) * metrics->height();

        auto height = std::max(height1, height2) + 100;

        QRect rect(620, y, 1416, height);
        QRect rect2(2036, y, 2300, height);

        painter.drawRect(rect);
        painter.drawRect(rect2);
        auto textRect = QRect(rect.x()+10, rect.y(), rect.width() - 10, rect.height());
        painter.drawText(textRect,  Qt::AlignVCenter | Qt::TextWordWrap, methods[i].methodName);
        auto textRect2 = QRect(rect2.x()+10, rect2.y(), rect2.width() - 10, rect2.height());
        painter.drawText(textRect2, Qt::AlignVCenter | Qt::TextWordWrap, methods[i].description);
        y+=height;
    }

    font.setPixelSize(85);
    delete metrics;
    metrics = new QFontMetrics(font);
    painter.setFont(font);

    auto repairCostString = QString("Стоимость : %1 руб.").arg(card.costForClient);
    auto x = 4958 - 620 - metrics->width(repairCostString);
    y+=165;
    painter.drawText(x, y, repairCostString);

    QString warantyText = "Гарантийный срок на ";
    auto warantyTextWidth = metrics->width(warantyText);
    auto doneText = "выполненную ";
    painter.drawText(620,y, warantyText);
    font.setBold(true);
    painter.setFont(font);
    auto boldMetrics = new QFontMetrics(font);
    auto doneTextWidth = boldMetrics->width(doneText);
    painter.drawText(620 + warantyTextWidth, y, doneText);
    font.setBold(false);
    delete  boldMetrics;
    painter.setFont(font);
    painter.drawText(620 + warantyTextWidth + doneTextWidth, y, "работу - ________");

    y+=400;
    s = QString("Прибор выдал: ");
    painter.drawText(620, y, s);
    painter.drawLine(620 + metrics->width(s)+20, y+metrics->height()/2, 4338,  y+metrics->height()/2);
    y+=400;
    s = QString("Прибор получил: ");
    painter.drawText(620, y, s);
    painter.drawLine(620 + metrics->width(s)+20, y+metrics->height()/2, 4338,  y+metrics->height()/2);
    y+=400;
    s = QString("Дата: ");
    painter.drawText(620, y, s);
    painter.drawLine(620 + metrics->width(s)+20, y+metrics->height()/2, 1750,  y+metrics->height()/2);

    painter.drawLine(1644, 2560+metrics->height()/2, 4338,  2560+metrics->height()/2);

    font.setBold(true);
    painter.setFont(font);

    painter.drawText(1187, 974, card.client.name);
    painter.drawText(1187, 1111, card.client.address);
    painter.drawText(1187, 1248, card.product.name);

    if (card.product.isOwen)
    {
        auto createDate = Helper::ParseBarcode(card.barCode);
        painter.drawText(1187, 1385, "20" + createDate.year);
        painter.drawText(1742, 1385, createDate.month);
    }
    else
    {
        painter.drawText(1187, 1385, card.year);
        painter.drawText(1742, 1385, card.month);
    }

    painter.drawText(3453, 1385, card.barCode);
    painter.drawText(620, 1713, "Жалобы");
    painter.drawText(620, 2751, "Причина отказа");
    painter.drawText(1644, 2580, card.repairer);

    delete metrics;
}
