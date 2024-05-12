#include "contentmanager.h"
#include "utils.h"

ContentManager::ContentManager(bool empty)
{
    if (empty)
        return;

    _bdir = check_and_setup_application_data();
    _settings = Settings::from_json(_bdir.absoluteFilePath(FN_SETTINGS));
    _bills = _save_file.load_bills();
}

ContentManager::~ContentManager()
{
    _settings.to_json(_bdir.absoluteFilePath(FN_SETTINGS));
    _save_file.save_bills(_bills);
}
