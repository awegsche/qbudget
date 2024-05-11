#ifndef SETTINGS_H
#define SETTINGS_H

#include <QList>
#include "category.h"
#include "currency.h"

class Settings
{
public:
    Settings();

    void to_json(QString const &filename) const;

    static Settings from_json(QString const &filename);

    Currency const *currency(QString const &name) const
    {
        auto found = std::find_if(_currencies.constBegin(),
                                  _currencies.constEnd(),
                                  [&name](const Currency *curr) { return (name == curr->name()); });
        if (found != _currencies.constEnd()) {
            return *found;
        }
        return _currencies[0]; // this should always be EUR
    }

    Category const &category(int idx) const { return _categories[idx]; }

    QList<Currency *>::ConstIterator cCurrenciesBegin() const { return _currencies.constBegin(); }
    QList<Currency *>::ConstIterator cCurrenciesEnd() const { return _currencies.constEnd(); }

    QList<Category>::ConstIterator cCategoriesBegin() const { return _categories.constBegin(); }
    QList<Category>::ConstIterator cCategoriesEnd() const { return _categories.constEnd(); }

private:
    QList<Category> _categories;
    QList<Currency *> _currencies;
};

#endif // SETTINGS_H
