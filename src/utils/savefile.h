#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <QFile>
#include <QJsonDocument>
#include <QString>
#include "bill.h"

class SaveFile
{
public:
    SaveFile();
    SaveFile(QString const &filename)
        : _filename(filename)
    {}

    void save_bills(QVector<Bill> const &bills) const
    {
        QFile file{_filename};

        if (!file.open(QIODevice::WriteOnly)) {
            return;
        }

        QJsonArray json_bills;

        for (auto const &bill : bills) {
            json_bills.push_back(bill.to_json());
        }

        file.write(QJsonDocument(json_bills).toJson());
    }

    QVector<Bill> load_bills()
    {
        QVector<Bill> bills;

        qDebug() << "loading bills from " << _filename;
        QFile file{_filename};
        if (!file.open(QIODevice::ReadOnly)) {
            return bills;
        }

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        auto json_bills = doc.array();
        for (auto const &bill : json_bills) {
            bills.push_back(Bill::from_json(bill.toObject()));
        }
        return bills;
    }

    bool exists() const { return QFile::exists(_filename); }

private:
    QString _filename;
};

#endif // SAVEFILE_H
