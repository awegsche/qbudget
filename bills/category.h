#ifndef CATEGORY_H
#define CATEGORY_H

#include <QColor>
#include <QJsonObject>
#include <QString>

class Category
{
public:
    Category();
    Category(QString const &name, QColor const &color)
        : _name(name)
        , _color(color)
    {}

    // ---- serialisation -------------------------------------------------------------------------
    QJsonObject to_json() const
    {
        QJsonObject ob;
        ob["color"] = _color.name();
        ob["name"] = _name;

        return ob;
    }

    static Category from_json(QJsonObject const &ob)
    {
        auto const name = ob["name"].toString();
        auto const color = QColor::fromString(ob["color"].toString());

        return {name, color};
    }

    // ---- Properties ----------------------------------------------------------------------------
    const QString &name() const { return _name; }
    const QColor &color() const { return _color; }

private:
    QString _name;
    QColor _color;
};

#endif // CATEGORY_H
