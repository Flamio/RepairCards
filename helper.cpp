#include "helper.h"

Helper::Helper()
{

}

BarcodeData Helper::ParseBarcode(const QString& barcode)
{
    BarcodeData data;
    data.year = barcode.mid(5,2);
    data.month = barcode.mid(7,2);
    return data;
}
