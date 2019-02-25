#ifndef MAINPRESENTER_H
#define MAINPRESENTER_H

#include <QObject>
#include "imainview.h"
#include "databaseconnector.h"

class MainPresenter : public QObject
{
    Q_OBJECT
public:
    explicit MainPresenter(QObject *parent = 0);

    void setMainView(IMainView *value);

    void setDatabaseConnector(const DatabaseConnector &value);

signals:

public slots:

private:
    IMainView* mainView;
    DatabaseConnector databaseConnector;
};

#endif // MAINPRESENTER_H
