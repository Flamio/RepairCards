#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include "imainview.h"
#include "databaseconnector.h"
#include "cardmethod.h"

class MainPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MainPresenter(QObject *parent = 0);

    void setMainView(IMainView *value);

    void setDatabaseConnector(const DatabaseConnector &value);

    void start();

signals:

public slots:

private slots:
    void onBarCodeFinish(QString);
    void onAdd(const RepairCard&, const QVector<CardMethod>&);
    void onNewCard();
    void onCancelAdding();
    void onNavigation(bool);

private:
    IMainView* mainView;
    DatabaseConnector databaseConnector;
    RepairCard repairCard;

};

#endif // MAINPRESENTER_H
