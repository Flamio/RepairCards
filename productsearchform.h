#ifndef PRODUCTSEARCHFORM_H
#define PRODUCTSEARCHFORM_H

#include <QDialog>
#include <QListWidgetItem>
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
    virtual void setProducts(QVector<Product>&);

private:
    Ui::ProductSearchForm *ui;

    QVector<Product> products;

signals:
    void searchProduct(const QString& number);
    void done(Product);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_name_textChanged(const QString &arg1);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
};

#endif // PRODUCTSEARCHFORM_H
