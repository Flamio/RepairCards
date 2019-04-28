#ifndef PRODUCTEDITFORM_H
#define PRODUCTEDITFORM_H

#include "edithandbookform.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include "productsearchpresenter.h"
#include "productsearchform.h"

class ProductEditForm : public EditHandbookForm
{
public:
    ProductEditForm(QWidget *parent = 0);
    virtual void setMode(FormMode mode);
    virtual void setProduct(Product& p);

protected:
    virtual void fillHandbookFields(Handbook** h);
    virtual void clearFieldsOnAdd();
    virtual void fillFieldsOnEdit(int index);

private:
    QLabel codeLabel;
    QLineEdit code;
    QCheckBox isOwenCheck;
    QPushButton searchProductButton;
    ProductSearchPresenter* productSearch;
    ProductSearchForm* searchForm;

    void onProductSearch();
    void onSearchDone();
    void onOwenCheck(bool checked);
};

#endif // PRODUCTEDITFORM_H
