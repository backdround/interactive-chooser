#include <QDebug>

#include <algorithm>
#include <set>

#include "viewmodel.h"

Viewmodel::Viewmodel(Model_interface& model) : model_(model) {
    connect(&model_, &Model_interface::item_inserted, this,
            &Viewmodel::item_inserted);
    connect(&model_, &Model_interface::item_erased, this,
            &Viewmodel::item_erased);

    auto items = model_.items();
    for (auto& item : items) {
        item_t new_item;
        new_item.id = item.id;
        new_item.weight = 0;
        new_item.name = item.name.c_str();

        if (item.description) {
            new_item.description = item.description.value().c_str();
        }
        items_[new_item.id] = new_item;
    }
    resort();
}

//////////////////////////////////////////////////////////////////////
// Qt Model

int Viewmodel::rowCount(const QModelIndex& parent) const {
    return sorted_items_.size();
}

QHash<int, QByteArray> Viewmodel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ID] = "id";
    roles[NAME] = "name";
    roles[WEIGHT] = "weight";

    auto declaration = model_.declaration();
    if (declaration.description) {
        roles[DESCRIPTION] = "description";
    }

    return roles;
}

QVariant Viewmodel::data(const QModelIndex& model_index, int role) const {
    if (!model_index.isValid()) {
        return {};
    }


    const auto& item = sorted_items_[model_index.row()];
    switch (role) {
        case ID:
            return {item.id};
        case NAME:
            return {item.name};
        case WEIGHT:
            return {item.weight};
        case DESCRIPTION:
            return item.description;
        default:
            return {};
    }
}

//////////////////////////////////////////////////////////////////////
// Item inserted / erased
void Viewmodel::item_inserted(int id) {
    // Check for already exist.
    if (items_.find(id) != items_.end()) {
        qWarning() << "Inserted id already exist";
        return;
    }

    // Get item with given id.
    auto item = model_.item(id);
    if (!item) {
        qWarning() << "Couldn't get inserted item";
        return;
    }

    // Create appropriate item.
    item_t new_item;
    new_item.id = item->id;
    new_item.weight = model_.calculate_weight(id, last_input);
    new_item.name = item->name.c_str();

    if (item->description) {
        new_item.description = item->description.value().c_str();
    }

    // Add to all items.
    items_[id] = new_item;

    // Search appropriate place.
    auto iterator_to_place =
        std::upper_bound(sorted_items_.cbegin(), sorted_items_.cend(), new_item);

    // Inserting item to qt model.
    auto index = iterator_to_place - sorted_items_.cbegin();
    beginInsertRows({}, index, index);
    sorted_items_.insert(iterator_to_place, new_item);
    endInsertRows();
}


void Viewmodel::item_erased(int id) {
    // Erase from all items.
    auto iterator = items_.find(id);
    if (iterator == items_.end()) {
        qWarning() << "Erased id doesn't exist";
        return;
    }
    auto item_copy = iterator->second;
    items_.erase(iterator);

    // Find iterator to item from sorted items.
    auto erased_iterator = sorted_items_.end();
    auto [lower, upper] =
        std::equal_range(sorted_items_.begin(), sorted_items_.end(), item_copy);
    for (auto current = lower; current != upper; current++) {
        if (current->id == id) {
            erased_iterator = current;
            break;
        }
    }

    if (erased_iterator == sorted_items_.end()) {
        qWarning() << "Erased id in sorted items doesn't exist";
        return;
    }

    // Erasing item from qt model.
    auto index = erased_iterator - sorted_items_.begin();
    beginRemoveRows({}, index, index);
    sorted_items_.erase(erased_iterator);
    endRemoveRows();
}


//////////////////////////////////////////////////////////////////////
// Actions
void Viewmodel::action(QVariant id) {
    if (!id.canConvert<int>()) {
        qWarning() << "Couldn't convert action id to int";
        return;
    }

    model_.action(id.value<int>());
}

void Viewmodel::user_input_changed(QVariant input) {
    if (!input.canConvert<QString>()) {
        qWarning() << "Couldn't convert user input to QString";
        return;
    }

    last_input = input.toString().toStdString();
    resort();
}


//////////////////////////////////////////////////////////////////////
// Private
void Viewmodel::resort() {
    // Update weights.
    for (auto& [id, item] : items_) {
        item.weight = model_.calculate_weight(id, last_input);
    }

    // Sort element.
    std::multiset<item_t> sorted_set;
    for (const auto& [id, item] : items_) {
        sorted_set.insert(item);
    }

    // Reset model items.
    beginResetModel();
    sorted_items_.clear();
    sorted_items_.resize(sorted_set.size());
    std::move(sorted_set.rbegin(), sorted_set.rend(), sorted_items_.begin());
    endResetModel();
}
