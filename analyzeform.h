#ifndef ANALYZEFORM_H
#define ANALYZEFORM_H

#include <QDialog>
#include "analyze.h"

namespace Ui {
class AnalyzeForm;
}

class AnalyzeForm : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyzeForm(QWidget *parent = 0);
    ~AnalyzeForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AnalyzeForm *ui;
    Analyze analyze;
};

#endif // ANALYZEFORM_H
