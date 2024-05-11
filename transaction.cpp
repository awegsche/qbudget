#include "transaction.h"

Transaction::Transaction() {}

Transaction::Transaction(const QString &name, Money const &value)
    : _name(name)
    , _value(value)
{}
