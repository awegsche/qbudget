#include "billviewmodel.h"

BillViewModel::BillViewModel(ContentManager *manager)
    : _manager(manager)
    , _currency_font("Fira Code", 10)
    , _font("Fira Sans", 9)
    , _month_bgcolor(128, 128, 128)
    , _day_bgcolor(192, 192, 192)
    , _bill_bgcolor(216, 216, 216)
{
    int month_index = 0;
    for (int y = 2024; y < 2025; ++y) {
        for (int m = 1; m <= 5; ++m) {
            _months.push_back(new MonthNode(month_index, QDate(y, m, 1), _manager));
            ++month_index;
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
    case MonthNode::MONTH_DEPTH: // month
    {
        const auto *month = static_cast<const MonthNode *>(parent_node);
        return createIndex(row, column, month->days[row]);
    }
    case DayNode::DAY_DEPTH: //day
    {
        const auto *day = static_cast<const DayNode *>(parent_node);
        return createIndex(row, column, day->bills[row]);
    }
    case BillNode::BILL_DEPTH: // bill
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
    case TransactionNode::TRANSACTION_DEPTH: {
        const auto *transaction = static_cast<const TransactionNode *>(child_node);
        const auto *parent = transaction->parent;
        return createIndex(parent->row, 0, parent);
    }
    case BillNode::BILL_DEPTH: {
        const auto *bill = static_cast<const BillNode *>(child_node);
        const auto *parent = bill->parent;
        return createIndex(parent->row, 0, parent);
    }
    case DayNode::DAY_DEPTH: {
        const auto *day = static_cast<const DayNode *>(child_node);
        const auto parent = day->parent;
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
    case MonthNode::MONTH_DEPTH: {
        const auto *month = static_cast<const MonthNode *>(parent_node);
        return month->days.size();
    }
    case DayNode::DAY_DEPTH: {
        const auto *day = static_cast<const DayNode *>(parent_node);
        return day->bills.size();
    }
    case BillNode::BILL_DEPTH: {
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
        case Qt::FontRole: {
            return _font;
        }
        case Qt::DisplayRole: {
            switch (node->depth()) {
            case MonthNode::MONTH_DEPTH: {
                const auto *monthnode = static_cast<const MonthNode *>(node);
                return monthnode->date.toString("MMM yyyy");
            }
            case DayNode::DAY_DEPTH: {
                const auto *daynode = static_cast<const DayNode *>(node);
                return daynode->date.toString("dd.");
            }
            case BillNode::BILL_DEPTH: {
                const auto *billnode = static_cast<const BillNode *>(node);
                return billnode->bill->name() + ". "
                       + QString::number(billnode->bill->sum(), 'f', 2) + " €";
            }
            case TransactionNode::TRANSACTION_DEPTH: {
                const auto *transnode = static_cast<const TransactionNode *>(node);
                return transnode->transaction->name();
            }
            }
        }
        default:
            return {};
        }
    }
    case 1: {
        switch (role) {
        case Qt::DisplayRole: {
            switch (node->depth()) {
            case MonthNode::MONTH_DEPTH: {
                const auto *monthnode = static_cast<const MonthNode *>(node);
                return QString::number(monthnode->sum(), 'f', 2) + " €";
            }
            case DayNode::DAY_DEPTH: {
                const auto *daynode = static_cast<const DayNode *>(node);
                return QString::number(daynode->sum(), 'f', 2) + " €";
            }
            case BillNode::BILL_DEPTH: {
                const auto *billnode = static_cast<const BillNode *>(node);
                return QString::number(billnode->bill->sum(), 'f', 2) + " €";
            }
            case TransactionNode::TRANSACTION_DEPTH: {
                const auto *transnode = static_cast<const TransactionNode *>(node);
                return QString::number(transnode->transaction->value());
            }
            }
        }
        case Qt::TextAlignmentRole:
            return Qt::AlignRight;
        case Qt::FontRole:
            return _currency_font;
        }

    default:
        return {};
    }

        return {};
    }
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
    for (auto *b : days) {
        delete b;
    }
}

void BillNode::update()
{
    transactions.clear();
    for (size_t tr_idx = 0; tr_idx < bill->size(); ++tr_idx) {
        const auto *tr = &(*bill)[tr_idx];
        transactions.push_back(new TransactionNode(this, tr, tr_idx));
    }
}

BillNode::~BillNode()
{
    for (auto *t : transactions) {
        delete t;
    }
}

void DayNode::update(const ContentManager *manager)
{
    bills.clear();
    for (size_t i = 0; i < manager->bills()->size(); ++i) {
        auto const &bill = manager->bill(i);
        if (bill.date() == date) {
            bills.push_back(new BillNode(this, &bill, i));
        }
    }
}

DayNode::~DayNode()
{
    for (auto *b : bills) {
        delete b;
    }
}
