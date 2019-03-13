#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include "imainview.h"
#include "addpresenter.h"
#include "databaseconnector.h"

class MainPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MainPresenter(QObject *parent = 0);

    void setMainView(IMainView *value);

    void setAddPresenter(AddPresenter *value);

    void setDbConnector(const DatabaseConnector &value);
    void start();

signals:

public slots:

private slots:
    void onNavigation(bool);
    void onAddComplete();
    void onAdd();
    void onDelete(int);
    void onEdit(int);
    void onEditComplete(int);

private:
    IMainView* mainView = nullptr;
    AddPresenter* addPresenter = nullptr;
    DatabaseConnector dbConnector;
    void showLastCard();
};

#endif // MAINPRESENTER_H
