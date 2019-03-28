#ifndef EDITHANDBOOKFORM_H
#define EDITHANDBOOKFORM_H

#include <QDialog>
#include "ihandbookeditview.h"
#include "formmode.h"

namespace Ui {
class EditHandbookForm;
}

class EditHandbookForm : public QDialog, public IHandbookEditView
{
    Q_OBJECT

public:
    explicit EditHandbookForm(QWidget *parent = 0);
    ~EditHandbookForm();

    virtual void setHandbooks(const QVector<Handbook*>&);
    virtual void showWindow();
    virtual void closeWindow();
    virtual void setMode(FormMode mode);
    virtual void setHandbook(int id);

signals:
    void add(Handbook*);
    void edit(Handbook*);
    void deleteHandbook(int id);

protected slots:
    void on_pushButton_3_clicked();

    virtual void on_handbook_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_del_clicked();

protected:
    Ui::EditHandbookForm *getUi() const;
    QVector<Handbook*>* getHandbooks();
    Handbook *getHandbook() const;

    virtual void fillHandbookFields(Handbook**);
    virtual void clearFieldsOnAdd();
    virtual void fillFieldsOnEdit(int currentIndex);

private:
    Ui::EditHandbookForm *ui;
    QVector<Handbook*> handbooks;
    FormMode mode = Editing;

    Handbook* handbook = nullptr;
};

#endif // EDITHANDBOOKFORM_H
