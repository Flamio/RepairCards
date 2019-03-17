#ifndef HELPER_H
#define HELPER_H

#include <QString>

struct BarcodeData
{
    QString year;
    QString month;
};

class Helper
{
public:
    Helper();

    static BarcodeData ParseBarcode(const QString& barcode);
    static int getFontSize(){return 10;}
};

#endif // HELPER_H
