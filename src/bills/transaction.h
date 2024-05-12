#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "money.h"

#include <QDate>
#include <QJsonObject>
#include <QString>

class Transaction
{
public:
    Transaction();
    Transaction(QString const &name, Money const &value);

    bool operator==(Transaction const &other) const { return other._name == _name; }

    // ---- properties ----------------------------------------------------------------------------
    float value() const { return _value.value(); }
    float eur_value(Settings const *settings) const { return _value.eur_value(settings); }
    QString const &name() const { return _name; }

    // ---- serialisation -------------------------------------------------------------------------
    QJsonObject to_json() const
    {
        QJsonObject ob;
        ob["name"] = _name;
        ob["value"] = _value.to_json();

        return ob;
    }

    static Transaction from_json(QJsonObject const &ob)
    {
        //auto map = ob.toVariantMap();

        return Transaction{ob["name"].toString(), Money::from_json(ob["value"])};
    }

    QString toString() const { return _name + " " + _value.to_string(); }

private:
    QString _name;
    Money _value;
};

#endif // TRANSACTION_H
