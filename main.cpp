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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseConnector dbConnector;
    if (!dbConnector.open())
    {
        QMessageBox msg;
        msg.setText("Не могу открыть базу данных!!");
        msg.exec();
        return 0;
    }

    AddPresenter addPresenter(&a);

    MainForm mainForm;    

    AddForm w(&mainForm);
    EditHandbookForm editRepairerForm(&w);
    EditHandbookForm editMethodForm(&w);
    ClientEditForm clientEditForm(&w);
    ProductEditForm productEditForm(&w);
    PastRepairsList prl(&w);

    addPresenter.setAddView(&w);
    addPresenter.setDatabaseConnector(dbConnector);
    addPresenter.setRepairerEditView(&editRepairerForm);
    addPresenter.setMethodEditView(&editMethodForm);
    addPresenter.setClientEditView(&clientEditForm);
    addPresenter.setProductEditView(&productEditForm);
    addPresenter.setPastPrepareList(&prl);
    addPresenter.start();


    MainPresenter mainPresenter;
    mainPresenter.setPastPrepareList(&prl);
    mainPresenter.setAddPresenter(&addPresenter);
    mainPresenter.setDbConnector(dbConnector);
    mainPresenter.setMainView(&mainForm);
    mainPresenter.start();


    return a.exec();
}
