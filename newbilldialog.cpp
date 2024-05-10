#include "newbilldialog.h"
#include "ui_newbilldialog.h"

#include <QCompleter>

NewBillDialog::NewBillDialog(QStringList const &names, QStringList const &accounts, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewBillDialog)
{
    ui->setupUi(this);

    auto name_comp = new QCompleter(names, this);
    name_comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->editName->setCompleter(name_comp);

    auto acc_comp = new QCompleter(accounts, this);
    acc_comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->editAccount->setCompleter(acc_comp);

    ui->editDate->setDate(QDate::currentDate());
}

NewBillDialog::~NewBillDialog()
{
    delete ui;
}

void NewBillDialog::on_buttonAdd_clicked()
{
    bill.insert_transaction(ui->editName->text(),
                            ui->editValue->value());

    ui->editName->clear();
    ui->editName->setFocus();

    update_summary();
}

void NewBillDialog::update_summary()
{
    QString summary = "";

    for (auto const &tr : bill) {
        summary += tr.toString() + "\n";
    }

    summary += "sum: " + QString::number(bill.sum());

    ui->labelSummary->setText(summary);
}

void NewBillDialog::on_editSum_editingFinished()
{
    if (ui->editSum->value() != 0.0) {
        bill.set_sum(ui->editSum->value());
    } else {
        bill.unset_sum();
    }
    update_summary();
}

void NewBillDialog::on_NewBillDialog_accepted()
{
    bill.set_account(ui->editAccount->text());
    bill.set_date(ui->editDate->date());
}
