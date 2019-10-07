#ifndef PRODUCTEDITFORM_H
#define PRODUCTEDITFORM_H

#include "edithandbookform.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include "handbooksearchform.h"

class ProductEditForm : public EditHandbookForm
{
public:
    ProductEditForm(QWidget *parent = 0);
    virtual void setMode(FormMode mode);
    virtual void setProduct(Product& p);

    void setGetProductsByNameFunc(const std::function<QVector<Product> (const QString&)> &value);

protected:
    virtual void fillHandbookFields(Handbook** h);
    virtual void clearFieldsOnAdd();
    virtual void fillFieldsOnEdit(int index);

private:
    QLabel codeLabel;
    QLineEdit code;
    QCheckBox isOwenCheck;
    QPushButton searchProductButton;
    HandbookSearchForm* searchForm;

    QVector<Product>* products = new QVector<Product>();

    std::function<QVector<Product>(const QString&)> getProductsByNameFunc = nullptr;

    void onProductSearch();
    void onSearchDone();
    void onOwenCheck(bool checked);
};

#endif // PRODUCTEDITFORM_H
