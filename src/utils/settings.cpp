#include "settings.h"
#include "utils.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Settings::Settings()
{
    _currencies.push_back(new Currency(1.0, "EUR", "€"));
    _currencies.push_back(new Currency(1.02, "CHF", "CHF"));
}

void Settings::to_json(const QString &filename) const
{
    QFile settings_file{filename};

    if (!settings_file.open(QIODevice::WriteOnly)) {
        return;
    }

    QJsonObject ob{};
    QJsonArray categories{};

    for (const auto &c : _categories) {
        categories.push_back(c.to_json());
    }
    ob["categories"] = categories;

    ob["data_dir"] = _data_dir.absolutePath();

    settings_file.write(QJsonDocument(ob).toJson());
}

Settings Settings::from_json(const QString &filename)
{
    QFile settings_file{filename};

    Settings s{};

    if (!settings_file.open(QIODevice::ReadOnly))
        return s;

    const auto ob = QJsonDocument::fromJson(settings_file.readAll()).object();

    if (ob.contains("categories")) {
        const auto arr = ob["categories"].toArray();

        for (const auto &c : arr) {
            s._categories.push_back(Category::from_json(c.toObject()));
        }
    }

    if (ob.contains("data_dir")) {
        s._data_dir = QDir(ob["data_dir"].toString());
    } else {
        s._data_dir = check_and_setup_application_data();
    }

    return s;
}
