#include "bill.h"

Bill::Bill() {}

void Bill::unset_sum()
{
    _sum_element.reset();
}

float Bill::eur_sum(const Settings *settings) const
{
    if (_sum_element.has_value()) {
        return _sum_element.value();
    }

    return std::accumulate(_transactions.constBegin(),
                           _transactions.constEnd(),
                           0.0f,
                           [settings](float sum, Transaction const &tr) {
                               return sum + tr.eur_value(settings);
                           });
}

bool Bill::operator==(const Bill &other) const
{
    if (other._transactions.size() != _transactions.size())
        return false;
    if (other._sum_element != _sum_element)
        return false;
    for (int i = 0; i < _transactions.size(); ++i) {
        if (other._transactions[i] != _transactions[i]) {
            return false;
        }
    }
    if (other._account != _account)
        return false;
    if (other._date != _date)
        return false;
    return true;
}

QJsonObject Bill::to_json() const
{
    QJsonObject ob;

    ob["name"] = _name;

    QJsonArray transactions;

    for (auto const &tr : *this) {
        transactions.push_back(tr.to_json());
    }

    ob["transactions"] = transactions;

    if (_sum_element.has_value()) {
        ob.insert("sum", _sum_element.value());
    }

    ob["date"] = _date.toString();
    ob["account"] = _account;

    QJsonArray tags;
    for (auto const &t : _tags) {
        tags.push_back(t);
    }
    ob["tags"] = tags;

    if (!_category.isEmpty()) {
        ob["category"] = _category;
    }

    return ob;
}

Bill Bill::from_json(const QJsonObject &ob)
{
    Bill b;
    if (ob.contains("name")) {
        b._name = ob["name"].toString();
    }
    if (ob.contains("sum")) {
        b._sum_element = ob["sum"].toDouble();
    }
    auto const &transactions = ob["transactions"].toArray();

    for (auto const &tr : transactions) {
        b._transactions.push_back(Transaction::from_json(tr.toObject()));
    }

    b._account = ob["account"].toString();
    b._date = QDate::fromString(ob["date"].toString());

    if (ob.contains("tags")) {
        for (auto const &t : ob["tags"].toArray()) {
            b._tags << t.toString();
        }
    }

    if (ob.contains("category")) {
        b._category = ob["category"].toString();
    }

    return b;
}
