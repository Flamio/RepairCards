#include "addform.h"
#include <QApplication>
#include <QMessageBox>
#include "databaseconnector.h"
#include "addpresenter.h"
#include "mainform.h"
#include "mainpresenter.h"
#include "edithandbookform.h"
#include "clienteditform.h"
#include "producteditform.h"
#include "pastrepairslist.h"
#include "handbooksearchform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto dbConnector = DatabaseConnector::getInstance();
    if (!dbConnector->open())
    {
        QMessageBox msg;
        msg.setText("Не могу открыть базу данных!");
        msg.exec();
        return 0;
    }

    dbConnector->createTables();
    dbConnector->convert();

    AddPresenter addPresenter(&a);

    MainForm mainForm;

    AddForm w;
    EditHandbookForm editRepairerForm(&w);
    EditHandbookForm editMethodForm(&w);
    ClientEditForm clientEditForm(&w);

    PastRepairsList prl(&w);

    HandbookSearchForm psf(&w);

    ProductEditForm productEditForm(&w);
    productEditForm.setGetProductsByNameFunc([&dbConnector](const QString& name) -> QVector<Product>
    {
        return dbConnector->getProductsByName(name);
    });

    addPresenter.setAddView(&w);
    addPresenter.setDatabaseConnector(*dbConnector);
    addPresenter.setRepairerEditView(&editRepairerForm);
    addPresenter.setMethodEditView(&editMethodForm);
    addPresenter.setClientEditView(&clientEditForm);
    addPresenter.setProductEditView(&productEditForm);
    addPresenter.setPastPrepareList(&prl);
    addPresenter.setProductSearch(&psf);
    addPresenter.start();

    MainPresenter mainPresenter;
    mainPresenter.setPastPrepareList(&prl);
    mainPresenter.setAddPresenter(&addPresenter);
    mainPresenter.setDbConnector(*dbConnector);
    mainPresenter.setMainView(&mainForm);

    HandbookSearchForm cardSearchForm;
    mainPresenter.setCardSearchView(&cardSearchForm);

    mainPresenter.start();


    return a.exec();
}
