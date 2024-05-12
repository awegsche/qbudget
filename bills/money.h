#ifndef MONEY_H
#define MONEY_H

#include "currency.h"
#include "settings.h"

#include <qjsonobject.h>

class Money
{
public:
    Money();
    Money(float value, QString const &currency)
        : _value(value)
        , _currency(currency)
    {}

    float value() const { return _value; }

    float eur_value(Settings const *settings) const
    {
        return settings->currency(_currency)->eur_value(_value);
    }

    QString to_string() const { return QString::number(_value) + " " + _currency; }

    QJsonObject to_json() const
    {
        QJsonObject ob;
        ob["value"] = _value;
        ob["currency"] = _currency;
        return ob;
    }

    static Money from_json(QJsonValue const &ob)
    {
        Money m;
        if (ob.isDouble()) {
            m._value = ob.toDouble();
            m._currency = "EUR";
        } else {
            m._value = ob["value"].toDouble();
            m._currency = ob["currency"].toString();
        }
        return m;
    }

private:
    float _value;
    QString _currency;
};

#endif // MONEY_H
