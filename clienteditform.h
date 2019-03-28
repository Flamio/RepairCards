#ifndef CLIENTEDITFORM_H
#define CLIENTEDITFORM_H

#include "edithandbookform.h"
#include <QLineEdit>
#include <QLabel>

class ClientEditForm : public EditHandbookForm
{
public:
    ClientEditForm(QWidget *parent = 0);

protected slots:
    virtual void on_handbook_currentIndexChanged(int index);

protected:
    void clearFieldsOnAdd();
    void fillHandbookFields(Handbook** h);
    virtual void fillFieldsOnEdit(int index);

private:
    QLabel phoneLabel;
    QLineEdit phone;

    QLabel contactLabel;
    QLineEdit contact;

    QLabel addressLabel;
    QLineEdit address;
};

#endif // CLIENTEDITFORM_H
