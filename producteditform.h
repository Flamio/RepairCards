#ifndef PRODUCTEDITFORM_H
#define PRODUCTEDITFORM_H

#include "edithandbookform.h"
#include <QLabel>
#include <QLineEdit>

class ProductEditForm : public EditHandbookForm
{
public:
    ProductEditForm(QWidget *parent = 0);

protected slots:
    virtual void on_handbook_currentIndexChanged(int index);

protected:
    virtual void fillHandbookFields(Handbook** h);
    virtual void clearFieldsOnAdd();

private:
    QLabel codeLabel;
    QLineEdit code;
};

#endif // PRODUCTEDITFORM_H
