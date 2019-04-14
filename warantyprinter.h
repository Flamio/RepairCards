#ifndef WARANTYPRINTER_H
#define WARANTYPRINTER_H

#include "printer.h"

class WarantyPrinter : public Printer
{
    Q_OBJECT
public:
    explicit WarantyPrinter(QObject *parent = 0);
    virtual ~WarantyPrinter();

signals:

public slots:

protected:
    virtual void drawContent(int id, QPainter&);

private:
    void drawText(QString bold, QString text, int& y);

    QPainter* painter = nullptr;
    QFont boldTextFont;
    QFont textFont;
    QFontMetrics* fm;
    QFontMetrics* fmBold;
};

#endif // WARANTYPRINTER_H
