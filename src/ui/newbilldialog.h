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
                           Settings const *settings,
                           QWidget *parent = nullptr);
    ~NewBillDialog();

    Bill const &get_bill() const { return bill; }

private slots:
    void on_buttonAdd_clicked();

    void on_editSum_editingFinished();

    void on_NewBillDialog_accepted();

    void on_comboCurrency_currentTextChanged(const QString &arg1);

    void on_comboCategory_currentIndexChanged(int index);

    void on_comboCategory_currentTextChanged(const QString &arg1);

    void on_actionAccept_triggered();

private:
    void update_summary();

    Ui::NewBillDialog *ui;
    Bill bill;
    Settings const *_settings;
};

#endif // NEWBILLDIALOG_H
