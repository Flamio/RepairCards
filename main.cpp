#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "databaseconnector.h"
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

    MainPresenter mainPresenter(&a);
    MainWindow w;
    mainPresenter.setMainView(&w);
    mainPresenter.setDatabaseConnector(dbConnector);
    w.show();

    mainPresenter.start();

    return a.exec();
}
