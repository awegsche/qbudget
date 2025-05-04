#ifndef SETTINGS_H
#define SETTINGS_H

#include <QList>
#include "category.h"
#include "currency.h"
#include <qdir.h>

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
    QDir const &data_dir() const { return _data_dir; }
    Category const &category(QString const &name) const
    {
        auto found = std::find_if(cCategoriesBegin(),
                                  cCategoriesEnd(),
                                  [&name](Category const &cat) { return cat.name() == name; });
        if (found == cCategoriesEnd())
            return _categories[0];
        return *found;
    }

    QList<Currency *>::ConstIterator cCurrenciesBegin() const { return _currencies.constBegin(); }
    QList<Currency *>::ConstIterator cCurrenciesEnd() const { return _currencies.constEnd(); }

    QList<Category>::ConstIterator cCategoriesBegin() const { return _categories.constBegin(); }
    QList<Category>::ConstIterator cCategoriesEnd() const { return _categories.constEnd(); }

private:
    QList<Category> _categories;
    QList<Currency *> _currencies;
    QDir _data_dir;
};

#endif // SETTINGS_H
