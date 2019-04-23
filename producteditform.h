#ifndef PRODUCTEDITFORM_H
#define PRODUCTEDITFORM_H

#include "edithandbookform.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "productsearchpresenter.h"
#include "productsearchform.h"

class ProductEditForm : public EditHandbookForm
{
public:
    ProductEditForm(QWidget *parent = 0);
    virtual void setMode(FormMode mode);

protected:
    virtual void fillHandbookFields(Handbook** h);
    virtual void clearFieldsOnAdd();
    virtual void fillFieldsOnEdit(int index);

private:
    QLabel codeLabel;
    QLineEdit code;
    QPushButton searchProductButton;
    ProductSearchPresenter* productSearch;
    ProductSearchForm* searchForm;

    void onProductSearch();
    void onSearchDone();
};

#endif // PRODUCTEDITFORM_H
