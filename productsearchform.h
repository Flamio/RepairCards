#ifndef PRODUCTSEARCHFORM_H
#define PRODUCTSEARCHFORM_H

#include <QDialog>
#include "iproductsearchview.h"

namespace Ui {
class ProductSearchForm;
}

class ProductSearchForm : public QDialog, public IProductSearchView
{
    Q_OBJECT

public:
    explicit ProductSearchForm(QWidget *parent = 0);
    ~ProductSearchForm();

    virtual void showWindow() {this->show();}
    virtual void closeWindow() {this->close();}
    void setName(const QString& name);

private:
    Ui::ProductSearchForm *ui;

signals:
    void searchProduct(const QString& number);
    void done();

private slots:
    void on_number_textChanged(const QString &arg1);
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // PRODUCTSEARCHFORM_H
