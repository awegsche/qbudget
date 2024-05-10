#include "billviewmodel.h"

BillViewModel::BillViewModel(ContentManager *manager)
    : _manager(manager)
{
    int month_index = 0;
    for (int y = 2024; y < 2025; ++y) {
        for (int m = 1; m <= 5; ++m) {
            auto *month = new MonthNode;
            month->row = month_index;
            month->date = QDate(y, m, 1);
            ++month_index;

            for (size_t i = 0; i < _manager->bills()->size(); ++i) {
                auto const &bill = _manager->bill(i);
                if (bill.date().year() == y && bill.date().month() == m) {
                    auto billmodel = new BillNode();
                    billmodel->row = i;
                    billmodel->parent = month;
                    billmodel->bill = &bill;
                    month->bills.push_back(billmodel);

                    for (size_t tr_idx = 0; tr_idx < bill.size(); ++tr_idx) {
                        auto trnode = new TransactionNode();
                        const auto *tr = &bill[tr_idx];
                        trnode->transaction = tr;
                        trnode->row = tr_idx;
                        trnode->parent = billmodel;
                        billmodel->transactions.push_back(trnode);
                    }
                }
            }
            _months.push_back(month);
        }
    }
}

QModelIndex BillViewModel::index(int row, int column, const QModelIndex &parent) const
{
    // if parent isn't valid, we're at top
    if (!parent.isValid()) {
        return createIndex(row, column, _months[row]);
    }
    const auto *parent_node = static_cast<const BillViewModelNode *>(parent.internalPointer());
    switch (parent_node->depth()) {
    case 1: // month
    {
        const auto *month = static_cast<const MonthNode *>(parent_node);
        return createIndex(row, column, month->bills[row]);
    }
    case 2: // bill
    {
        const auto *bill = static_cast<const BillNode *>(parent_node);
        return createIndex(row, column, bill->transactions[row]);
    }
    }

    return {};
}

QModelIndex BillViewModel::parent(const QModelIndex &child) const
{
    const auto *child_node = static_cast<const BillViewModelNode *>(child.internalPointer());

    switch (child_node->depth()) {
    case 3: {
        const auto *transaction = static_cast<const TransactionNode *>(child_node);
        const auto *parent = transaction->parent;
        return createIndex(parent->row, 0, parent);
    }
    case 2: {
        const auto *bill = static_cast<const BillNode *>(child_node);
        const auto *parent = bill->parent;
        return createIndex(parent->row, 0, parent);
    }
    }
    return {};
}

int BillViewModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return _months.size();

    const auto *parent_node = static_cast<const BillViewModelNode *>(parent.constInternalPointer());
    switch (parent_node->depth()) {
    case 1: {
        const auto *month = static_cast<const MonthNode *>(parent_node);
        return month->bills.size();
    }
    case 2: {
        const auto *bill = static_cast<const BillNode *>(parent_node);
        return bill->transactions.size();
    }
    }

    return 0;
}

int BillViewModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant BillViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto *node = static_cast<const BillViewModelNode *>(index.constInternalPointer());

    switch (index.column()) {
    case 0: {
        switch (role) {
        case Qt::DisplayRole: {
            switch (node->depth()) {
            case 1: {
                const auto *monthnode = static_cast<const MonthNode *>(node);
                return monthnode->date.toString("MMM yyyy");
            }
            case 2: {
                const auto *billnode = static_cast<const BillNode *>(node);
                return billnode->bill->date().toString("dd")
                       + ". sum: " + QString::number(billnode->bill->sum());
            }
            case 3: {
                const auto *transnode = static_cast<const TransactionNode *>(node);
                return transnode->transaction->name();
            }
            }
        }
        default:
            return {};
        }
    }
    default:
        return {};
    }

    return {};
}

/*
bool BillViewModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    const auto *parent_node = static_cast<const BillViewModelNode *>(parent.constInternalPointer());
    switch (parent_node->depth()) {
    case 1: {
        const auto *month = static_cast<const MonthNode *>(parent_node);
        return month->bills.size() > 0;
    }
    case 2: {
        const auto *bill = static_cast<const BillNode *>(parent_node);
        return bill->transactions.size() > 0;
    }
    }

    return false;
}
*/

MonthNode::~MonthNode()
{
    for (auto *b : bills) {
        delete b;
    }
}

BillNode::~BillNode()
{
    for (auto *t : transactions) {
        delete t;
    }
}
