#ifndef BILL_H
#define BILL_H

#include "settings.h"
#include "transaction.h"

#include <QJsonArray>

class Bill
{
public:
    Bill();

    Bill(QString const &account, QDate const &date)
        : _account(account)
        , _date(date)
    {}

    typedef QVector<Transaction>::ConstIterator iterator;

    // ---- access --------------------------------------------------------------------------------

    /*
     * Inserts new transaction
     */
    void insert_transaction(QString const &name, Money const &value)
    {
        _transactions.emplaceBack(name, value);
    }

    /*
     * Sets the sum_element.
     */
    void set_sum(float value) { _sum_element = value; }

    /*
     * Unsets the sum_element.
     */
    void unset_sum();

    void set_account(QStringView account) { _account = account.toString(); }
    void set_date(QDate const &date) { _date = date; }

    // ---- properties ----------------------------------------------------------------------------
    float eur_sum(Settings const *settings) const
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

    QDate const &date() const { return _date; }

    QString const &account() const { return _account; }

    bool operator==(const Bill &other) const
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

    // ---- stl container -------------------------------------------------------------------------
    float size() const { return _transactions.size(); }
    iterator begin() const { return _transactions.begin(); }
    iterator end() const { return _transactions.end(); }

    // ---- serialisation -------------------------------------------------------------------------
    QJsonObject to_json() const
    {
        QJsonObject ob;

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

        if (_category != -1) {
            ob["category"] = _category;
        }

        return ob;
    }

    static Bill from_json(QJsonObject const &ob)
    {
        Bill b;
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
            b._category = ob["category"].toInteger();
        }

        return b;
    }

    QString const &name() const
    {
        if (_name)
            return _name.value();

        return _transactions[0].name();
    }

    Transaction const &operator[](size_t idx) const { return _transactions[idx]; }
    Transaction &operator[](size_t idx) { return _transactions[idx]; }

private:
    std::optional<QString> _name = {};
    QVector<Transaction> _transactions = {};
    int _category = -1;
    QString _account = "";
    QDate _date = {};
    std::optional<float> _sum_element = {};
    QStringList _tags = {};
};

#endif // BILL_H
