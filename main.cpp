#include "addform.h"
#include <QApplication>
#include <QMessageBox>
#include "databaseconnector.h"
#include "addpresenter.h"
#include "mainform.h"
#include "mainpresenter.h"

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
    mainForm.show();

    AddForm w(&mainForm);
    addPresenter.setAddView(&w);
    addPresenter.setDatabaseConnector(dbConnector);
    addPresenter.start();

    MainPresenter mainPresenter;
    mainPresenter.setAddPresenter(&addPresenter);
    mainPresenter.setDbConnector(dbConnector);
    mainPresenter.setMainView(&mainForm);
    mainPresenter.start();


    return a.exec();
}
