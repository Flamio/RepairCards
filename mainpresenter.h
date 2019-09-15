#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include "imainview.h"
#include "addpresenter.h"
#include "databaseconnector.h"
#include "ihandbookeditview.h"
#include "ipastrepairlist.h"
#include "printtype.h"
#include "printerfactory.h"

class MainPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MainPresenter(QObject *parent = 0);
    virtual ~MainPresenter();

    void setMainView(IMainView *value);

    void setAddPresenter(AddPresenter *value);

    void setDbConnector(const DatabaseConnector &value);
    void start();

    void setPastPrepareList(IPastRepairList *value);

signals:

public slots:

private slots:
    void onNavigation(bool);
    void onAddComplete();
    void onAdd();
    void onDelete(int);
    void onEdit(int);
    void onEditComplete(int);
    void onCardClicked(int);
    void onPrint(int id,PrintType::PrintType&);
    void onShowSendedProducts();
    void onShowExtremeCard(ExtremeCardType::ExtremeCardType);
    void onShowCardByIndex(int index);

private:
    IMainView* mainView = nullptr;
    IPastRepairList* pastPrepareList = nullptr;

    AddPresenter* addPresenter = nullptr;
    DatabaseConnector dbConnector;
    PrinterFactory* printerFactory = nullptr;

    void showLastCard();
};

#endif // MAINPRESENTER_H
