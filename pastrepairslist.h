#ifndef PASTREPAIRSLIST_H
#define PASTREPAIRSLIST_H

#include <QDialog>
#include "ipastrepairlist.h"

namespace Ui {
class PastRepairsList;
}

class PastRepairsList : public QDialog, public IPastRepairList
{
    Q_OBJECT

public:
    explicit PastRepairsList(QWidget *parent = 0);
    ~PastRepairsList();

    void showWindow();
    void closeWindow();

    void setCards(QVector<RepairCard>&);

signals:
    void cardClicked(int id);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PastRepairsList *ui;

    QVector<RepairCard> cards;
};

#endif // PASTREPAIRSLIST_H
