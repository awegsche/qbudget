#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newbilldialog.h"
#include "utils.h"

inline constexpr const char *FN_METADATA = "metadata.json";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const auto bdir = check_and_setup_application_data();

    _contentmanager = std::make_unique<ContentManager>();

    _sum_graph = new QCPBars(ui->plotPanel->xAxis, ui->plotPanel->yAxis);

    // ---- metadata ------------------------------------------------------------------------------
    QFile meta_data{bdir.absoluteFilePath(FN_METADATA)};

    if (meta_data.open(QIODevice::ReadOnly)) {
        auto doc = QJsonDocument::fromJson(meta_data.readAll()).object();
        if (doc.contains("account_names")) {
            const auto accnames = doc["account_names"].toArray();
            for (const auto &an : accnames) {
                _account_names.push_back(an.toString());
            }
        }
        if (doc.contains("transaction_names")) {
            for (const auto &an : doc["transaction_names"].toArray()) {
                _transaction_names.push_back(an.toString());
            }
        }
    }

    // temporary
    for (auto const &b : *_contentmanager->bills()) {
        if (!_account_names.contains(b.account())) {
            _account_names.push_back(b.account());
        }
        for (auto const &tr : b) {
            if (!_transaction_names.contains(tr.name())) {
                _transaction_names.push_back(tr.name());
            }
        }
    }
    update_display();

    _billviewmodel = std::make_unique<BillViewModel>(_contentmanager.get());
    ui->tableBills->setModel(_billviewmodel.get());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

void MainWindow::on_actionNew_Bill_triggered()
{
    NewBillDialog dialog{_transaction_names, _account_names};

    if (dialog.exec() == QDialog::Accepted) {
        _contentmanager->bills()->push_back(dialog.get_bill());
        update_display();
    }
}

void MainWindow::update_display()
{
    _sum_graph->setData({}, {});

    QString display = display_month(0) + "<br/>" + display_month(1);
    ui->display->setHtml(display);
    ui->plotPanel->rescaleAxes();
    ui->plotPanel->yAxis->setRange(0.0, 10000.0);
}

QString MainWindow::display_month(int deltat_month) const
{
    const static float monthly_budget = 1700.0f;
    const auto target_date = QDate::currentDate().addMonths(-deltat_month);

    size_t count = 0;
    const auto *bills = _contentmanager->bills();
    const float sum = std::accumulate(bills->begin(),
                                      bills->end(),
                                      0.0f,
                                      [target_date, &count](float s, const Bill &bill) {
                                          if (bill.date().month() == target_date.month()
                                              && bill.date().year() == target_date.year()) {
                                              ++count;
                                              return s + bill.sum();
                                          }
                                          return s;
                                      });

    QString display = "<h1>" + target_date.toString("MMM yyyy") + "</h1>";

    display += QString::number(count) + " bills<br/>";
    display += "sum: " + QString::number(sum) + " € <font color=gray>("
               + QString::number(sum * 100.0 / monthly_budget, 'f', 2)
               + " % of monthly budget)</font>";
    _sum_graph->addData(-(double) deltat_month, sum);

    return display;
}

void MainWindow::on_textEdit_textChanged()
{
    ui->textBrowser->setHtml(ui->textEdit->document()->toRawText());
}
