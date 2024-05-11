#include "newbilldialog.h"
#include "currency.h"
#include "ui_newbilldialog.h"

#include <QCompleter>
#include <QShortcut>

NewBillDialog::NewBillDialog(QStringList const &names,
                             QStringList const &accounts,
                             const Settings *settings,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewBillDialog)
    , _settings(settings)
{
    ui->setupUi(this);

    auto name_comp = new QCompleter(names, this);
    name_comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->editName->setCompleter(name_comp);

    auto acc_comp = new QCompleter(accounts, this);
    acc_comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->editAccount->setCompleter(acc_comp);

    ui->editDate->setDate(QDate::currentDate());

    for (auto it = settings->cCurrenciesBegin(); it != settings->cCurrenciesEnd(); ++it) {
        ui->comboCurrency->addItem((*it)->name());
    }

    for (auto it = settings->cCategoriesBegin(); it != settings->cCategoriesEnd(); ++it) {
        ui->comboCategory->addItem(it->name());
        ui->comboCategory->setItemData(ui->comboCategory->count() - 1,
                                       QBrush(it->color()),
                                       Qt::ForegroundRole);
    }

    QShortcut *sc = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_J), this);
    QObject::connect(sc, &QShortcut::activated, this, &NewBillDialog::accept);
}

NewBillDialog::~NewBillDialog()
{
    delete ui;
}

void NewBillDialog::on_buttonAdd_clicked()
{
    bill.insert_transaction(ui->editName->text(),
                            Money(ui->editValue->value(),
                                  ui->comboCurrency->currentText())); // todo: add currency dropdown

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

    summary += "sum: " + QString::number(bill.eur_sum(_settings));

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
    if (!ui->editBillName->text().isEmpty()) {
        bill.set_name(ui->editBillName->text());
    }
    bill.set_account(ui->editAccount->text());
    bill.set_date(ui->editDate->date());
    bill.set_category(ui->comboCategory->currentText());
}

void NewBillDialog::on_comboCurrency_currentTextChanged(const QString &arg1)
{
    auto const *currency = _settings->currency(arg1);

    ui->editSum->setSuffix(currency->symbol());
    ui->editValue->setSuffix(currency->symbol());
}

void NewBillDialog::on_comboCategory_currentIndexChanged(int index)
{
    const auto color = _settings->category(index).color();
    QPalette pal = ui->comboCategory->palette();
    pal.setColor(QPalette::Text, color);
    ui->comboCategory->setPalette(pal);
}

void NewBillDialog::on_comboCategory_currentTextChanged(const QString &arg1) {}

void NewBillDialog::on_actionAccept_triggered()
{
    ui->labelSummary->setText("fsejjjjjjjjjjjjjjjjjjjjjjj");
    emit accepted();
}
