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

    void set_name(QString const &name) { _name = name; }

    void set_account(QStringView account) { _account = account.toString(); }
    void set_date(QDate const &date) { _date = date; }

    // ---- properties ----------------------------------------------------------------------------
    float eur_sum(Settings const *settings) const;

    QDate const &date() const { return _date; }

    QString const &account() const { return _account; }

    bool operator==(const Bill &other) const;

    QString const &name() const
    {
        if (!_name.isEmpty())
            return _name;

        if (_transactions.size() > 0)
            return _transactions[0].name();

        return _name;
    }

    QString const &category() const { return _category; }

    // ---- stl container -------------------------------------------------------------------------
    float size() const { return _transactions.size(); }
    iterator begin() const { return _transactions.begin(); }
    iterator end() const { return _transactions.end(); }

    // ---- serialisation -------------------------------------------------------------------------
    QJsonObject to_json() const;

    static Bill from_json(QJsonObject const &ob);

    // ---- access --------------------------------------------------------------------------------
    Transaction const &operator[](size_t idx) const { return _transactions[idx]; }
    Transaction &operator[](size_t idx) { return _transactions[idx]; }

    void set_category(QString const &category) { _category = category; }

private:
    QString _name = "";
    QVector<Transaction> _transactions = {};
    QString _category = "";
    QString _account = "";
    QDate _date = {};
    std::optional<float> _sum_element = {};
    QStringList _tags = {};
};

#endif // BILL_H
