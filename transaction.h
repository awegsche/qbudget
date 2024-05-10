#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QDate>
#include <QJsonObject>
#include <QString>

class Transaction
{
public:
    Transaction();
    Transaction(QString const &name, float value);

    bool operator==(Transaction const &other) const = default;

    // ---- properties ----------------------------------------------------------------------------
    float value() const { return _value; }

    QString const &name() const { return _name; }

    // ---- serialisation -------------------------------------------------------------------------
    QJsonObject to_json() const
    {
        QJsonObject ob;
        ob.insert("name", _name);
        ob.insert("value", _value);

        return ob;
    }

    static Transaction from_json(QJsonObject const &ob)
    {
        auto map = ob.toVariantMap();

        return Transaction{
            map["name"].toString(),
            map["value"].toFloat(),
        };
    }

    QString toString() const { return QString("%1 %2").arg(_name, QString::number(_value)); }

private:
    QString _name;
    float _value;
};

#endif // TRANSACTION_H
