#include <QDebug>

#include <list>
#include <filesystem>

#include <libxdgbasedir/libxdgbasedir.h>
#include "fuzzywuzzy.hpp"

#include "application_model.h"

Application_model::Application_model() {
    namespace fs = std::filesystem;

    // Get applications directories.
    std::list<fs::path> application_directory_paths;
    auto data_path_strings = xdg::data::dirs();
    data_path_strings.push_back(xdg::data::home());
    for (const auto& path_string : data_path_strings) {
        fs::path path{path_string};
        path /= "applications/";
        if(fs::is_directory(path)) {
            application_directory_paths.push_back(path);
        }
    }

    // Get applications paths.
    std::list<fs::path> application_paths;
    for (const auto& directory : application_directory_paths) {
        for (const auto& directory_entry : fs::directory_iterator(directory)) {
            application_paths.push_back(directory_entry.path());
        }
    }

    // Create model.
    int id = 0;
    for (const auto& path : application_paths) {
        auto name = path.filename().string();
        item_t item{.id = id++, .name = name, .description = {}};
        items_[item.id] = item;
    }
}

std::list<item_t> Application_model::items() {
    std::list<item_t> return_list;
    for (auto& [_, item] : items_) {
        return_list.push_back(item);
    }
    return return_list;
}

std::optional<item_t> Application_model::item(int id) {
    if (items_.find(id) == items_.end()) {
        return {};
    }
    return items_[id];
}

void Application_model::action(int id) {
}

int Application_model::calculate_weight(int id, const std::string& sort_string) {
    auto iterator = items_.find(id);
    if (iterator == items_.end()) {
        qWarning() << "Couldn't find id for weight calculation";
        return -1;
    }

    const auto& item = iterator->second;
    return fuzz::ratio(item.name, sort_string);
}
