#include "stub_model.h"

#include <iostream>
using namespace std;

Stub_model::Stub_model() {
    items_[0] = {0, "one",   {}};
    items_[1] = {1, "two",   {}};
    items_[2] = {2, "three", {}};
    items_[3] = {3, "four",  {}};
    items_[4] = {4, "five",  {}};
}

std::list<item_t> Stub_model::items() {
    std::list<item_t> return_list;
    for (auto& [_, item] : items_) {
        return_list.push_back(item);
    }
    return return_list;
}

std::optional<item_t> Stub_model::item(int id) {
    if (items_.find(id) == items_.end()) {
        return {};
    }
    return items_[id];
}

void Stub_model::action(int id) {
    cout << "action index: " << id << endl;
}

void Stub_model::user_input_changed(std::string input) {
    cout << "user input: " << input << endl;
}
