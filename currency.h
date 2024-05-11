#ifndef CURRENCY_H
#define CURRENCY_H

#include <qstring.h>

class Currency
{
public:
    Currency();
    Currency(float relative_value, QString const &name, QString const &symbol)
        : _relative_value(relative_value)
        , _name(name)
        , _symbol(symbol)
    {}

    float eur_value(float value) const { return _relative_value * value; }

    QString const &name() const { return _name; }
    QString const &symbol() const { return _symbol; }

private:
    float _relative_value;
    QString _name;
    QString _symbol;
};

#endif // CURRENCY_H
