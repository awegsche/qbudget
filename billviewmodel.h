#ifndef BILLVIEWMODEL_H
#define BILLVIEWMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "qfont.h"
#include <Bill.h>
#include <ContentManager.h>

struct BillViewModelNode
{
    BillViewModelNode(int r)
        : row(r)
    {}
    virtual ~BillViewModelNode(){};
    virtual int depth() const = 0;
    int row;
};

struct MonthNode;
struct DayNode;
struct BillNode;
struct TransactionNode;

struct TransactionNode : public BillViewModelNode
{
    TransactionNode(BillNode *p, Transaction const *tr, int index)
        : BillViewModelNode(index)
        , parent(p)
        , transaction(tr)
    {}

    BillNode *parent;
    Transaction const *transaction;

    constexpr static int TRANSACTION_DEPTH = 4;
    int depth() const override { return TRANSACTION_DEPTH; }
};

struct BillNode : public BillViewModelNode
{
    BillNode(DayNode *p, Bill const *b, int index)
        : BillViewModelNode(index)
        , parent(p)
        , bill(b)
    {
        update();
    }

    void update();

    ~BillNode();
    DayNode *parent;
    Bill const *bill;
    QList<TransactionNode *> transactions;

    constexpr static int BILL_DEPTH = 3;
    int depth() const override { return BILL_DEPTH; }
};

struct DayNode : public BillViewModelNode
{
    DayNode(MonthNode *p, int index, QDate const &date, ContentManager const *manager)
        : BillViewModelNode(index)
        , parent(p)
        , date(date)
    {
        update(manager);
    }

    void update(ContentManager const *manager);

    ~DayNode();

    MonthNode *parent;
    QList<BillNode *> bills;
    QDate date;

    float eur_sum(Settings const *settings) const
    {
        return std::accumulate(bills.constBegin(),
                               bills.constEnd(),
                               0.0f,
                               [settings](float sum, BillNode const *bn) {
                                   return sum + bn->bill->eur_sum(settings);
                               });
    }

    constexpr static int DAY_DEPTH = 2;
    int depth() const override { return DAY_DEPTH; }
};

struct MonthNode : public BillViewModelNode
{
    MonthNode(int index, QDate const &date, ContentManager const *manager)
        : BillViewModelNode(index)
        , date(date)
    {
        update(manager);
    }
    ~MonthNode();

    void update(ContentManager const *manager)
    {
        days.clear();
        int d_idx = 0;
        for (int d = 1; d <= 31; ++d) {
            const QDate day{date.year(), date.month(), d};
            if (!day.isValid())
                continue;

            auto daynode = new DayNode(this, d_idx, day, manager);
            ++d_idx;

            if (!daynode->bills.empty())
                days.push_back(daynode);
        }
    }

    QDate date;
    QList<DayNode *> days;

    float eur_sum(Settings const *settings) const
    {
        return std::accumulate(days.constBegin(),
                               days.constEnd(),
                               0.0f,
                               [settings](float sum, const DayNode *dn) {
                                   return sum + dn->eur_sum(settings);
                               });
    }

    constexpr static int MONTH_DEPTH = 1;
    int depth() const override { return MONTH_DEPTH; }
};

class BillViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    BillViewModel(ContentManager *manager);

    void update_month(const QDate &date)
    {
        for (auto *month : _months) {
            if (month->date.year() == date.year() && month->date.month() == date.month()) {
                month->update(_manager);
            }
        }
    }
    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    ContentManager *_manager;
    QList<MonthNode *> _months;
    QFont _currency_font;
    QFont _bold_currency_font;
    QFont _tiny_currency_font;
    QFont _font;
    QColor _month_bgcolor;
    QColor _day_bgcolor;
    QColor _bill_bgcolor;
    QColor _transaction_bgcolor;

    // QAbstractItemModel interface
public:
    //bool hasChildren(const QModelIndex &parent) const;
};

/*
quintptr make_id(quintptr date_id, quintptr bill_idx, quintptr transaction_idx);

quintptr get_date_idx(quintptr id);

quintptr get_bill_idx(quintptr id);

quintptr get_transaction_idx(quintptr id);
*/

#endif // BILLVIEWMODEL_H
