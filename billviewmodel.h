#ifndef BILLVIEWMODEL_H
#define BILLVIEWMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <Bill.h>
#include <ContentManager.h>

struct BillViewModelNode
{
    virtual int depth() const = 0;
    int row;
};

struct BillNode;
struct TransactionNode;
struct MonthNode;

struct TransactionNode : public BillViewModelNode
{
    BillNode *parent;
    Transaction const *transaction;

    int depth() const override { return 3; }
};

struct BillNode : public BillViewModelNode
{
    ~BillNode();
    MonthNode *parent;
    Bill const *bill;
    QList<TransactionNode *> transactions;

    int depth() const override { return 2; }
};

struct MonthNode : public BillViewModelNode
{
    ~MonthNode();

    QDate date;
    QList<BillNode *> bills;

    int depth() const override { return 1; }
};

class BillViewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    BillViewModel(ContentManager *manager);

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
