#include "transaction.h"

Transaction::Transaction() {}

Transaction::Transaction(const QString &name, float value)
    : _name(name)
    , _value(value)
{}
