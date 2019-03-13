#ifndef ADDPRESENTER_H
#define ADDPRESENTER_H

#include <QObject>
#include "iaddview.h"
#include "databaseconnector.h"
#include "cardmethod.h"

class AddPresenter : public QObject
{
    Q_OBJECT
public:
    explicit AddPresenter(QObject *parent = 0);

    void setAddView(IAddView *value);
    void setDatabaseConnector(const DatabaseConnector &value);
    void start();    

    IAddView *getAddView() const;

signals:
    void addComplete();
    void editComplete(int id);

public slots:

private slots:
    void onBarCodeFinish(QString);
    void onAdd(const RepairCard&, const QVector<CardMethod>&);
    void onEdit(const RepairCard&, const QVector<CardMethod>&);

private:
    IAddView* addView;
    DatabaseConnector databaseConnector;
};

#endif // ADDPRESENTER_H
