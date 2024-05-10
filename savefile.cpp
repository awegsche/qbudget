#include "savefile.h"
#include <QDir>
#include <QStandardPaths>

SaveFile::SaveFile()
{
    // check if we have a budget dir
    const auto bdir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    if (!bdir.exists()) {
        if (!bdir.mkpath(bdir.path())) {
            qDebug() << "error: can't create budget file";
        }
    }

    _filename = bdir.absoluteFilePath("budget.json");
}
