#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include "bill.h"
#include "savefile.h"
#include "settings.h"

#include <QDir>

constexpr const char *FN_SETTINGS = "settings.json";

class ContentManager
{
public:
    ContentManager(bool empty = false);
    ~ContentManager();

    QVector<Bill> *bills_mut() { return &_bills; }
    QVector<Bill> const *bills() const { return &_bills; }
    SaveFile *save_file() { return &_save_file; }

    const Bill &bill(size_t idx) const { return _bills[idx]; }
    Bill &bill_mut(size_t idx) { return _bills[idx]; }

private:
    // content
    QVector<Bill> _bills;
    SaveFile _save_file;
    Settings _settings;
    QDir _bdir;
};

#endif // CONTENTMANAGER_H
