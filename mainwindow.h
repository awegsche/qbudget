#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bill.h"
#include "billviewmodel.h"
#include "contentmanager.h"
#include "savefile.h"
#include "settings.h"

#include <QMainWindow>

#include <qcustomplot-source/qcustomplot.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionNew_Bill_triggered();

    void on_textEdit_textChanged();

private:
    void update_display();
    QString display_month(int deltat_month) const;

    // settings and metadata
    Settings _settings;
    QStringList _account_names;
    QStringList _transaction_names;

    // content
    std::unique_ptr<ContentManager> _contentmanager;
    std::unique_ptr<BillViewModel> _billviewmodel;

    // ui
    Ui::MainWindow *ui;

    // plot stuff
    QCPBars *_sum_graph;
};
#endif // MAINWINDOW_H
