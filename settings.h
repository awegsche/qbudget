#ifndef SETTINGS_H
#define SETTINGS_H

#include <QList>
#include "category.h"

class Settings
{
public:
    Settings();

    void to_json(QString const &filename) const;

    static Settings from_json(QString const &filename);

private:
    QList<Category> _categories;
};

#endif // SETTINGS_H
