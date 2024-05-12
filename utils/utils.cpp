#include "utils.h"

#include <QStandardPaths>
QDir check_and_setup_application_data()
{
    const auto bdir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

    if (!bdir.exists()) {
        if (!bdir.mkpath(bdir.path())) {
            qDebug() << "error: can't create budget file";
        }
    }
    return bdir;
}
