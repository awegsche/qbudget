#ifndef NEWBILLDIALOG_H
#define NEWBILLDIALOG_H

#include "bill.h"

#include <QDialog>

namespace Ui {
class NewBillDialog;
}

class NewBillDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewBillDialog(QStringList const &names,
                           QStringList const &accounts,
                           QWidget *parent = nullptr);
    ~NewBillDialog();

    Bill const &get_bill() const { return bill; }

private slots:
    void on_buttonAdd_clicked();

    void on_editSum_editingFinished();

    void on_NewBillDialog_accepted();

private:
    void update_summary();

    Ui::NewBillDialog *ui;
    Bill bill;
};

#endif // NEWBILLDIALOG_H
