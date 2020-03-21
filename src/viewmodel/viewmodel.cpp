#include <QDebug>

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
        new_item.name = item.name.c_str();

        if (item.description) {
            new_item.description = item.description.value().c_str();
        }
        items_.push_back(new_item);
    }
}

int Viewmodel::rowCount(const QModelIndex& parent) const {
    return items_.size();
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

    int index = model_index.row();
    switch (role) {
        case ID:
            return {items_[index].id};
        case NAME:
            return {items_[index].name};
        case DESCRIPTION:
            return items_[index].description;
        default:
            return {};
    }
}

void Viewmodel::item_inserted(int id) {
    // Get item with given id.
    auto item = model_.item(id);
    if (!item) {
        qWarning() << "Couldn't get inserted element";
        return;
    }

    // Create appropriate item.
    item_t new_item;
    new_item.id = item->id;
    new_item.name = item->name.c_str();

    if (item->description) {
        new_item.description = item->description.value().c_str();
    }

    // Inserting item to qt model.
    auto size = items_.size();
    beginInsertRows({}, size, size);
    items_.push_back(new_item);
    endInsertRows();
}


void Viewmodel::item_erased(int id) {
    // Find item with given id.
    auto index_optional = find(id);
    if (!index_optional) {
        qWarning() << "Couldn't find erased element";
        return;
    }
    auto index = index_optional.value();

    // Erasing item from qt model.
    auto erased_item = items_.begin() + index;
    beginRemoveRows({}, index, index);
    items_.erase(erased_item);
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

#include <iostream>
using namespace std;
void Viewmodel::user_input_changed(QVariant input) {
    if (!input.canConvert<QString>()) {
        qWarning() << "Couldn't convert user input to QString";
        return;
    }

    auto input_string = input.toString().toStdString();
    //model_.user_input_changed(input_string);
    cout << "input_string: " << input_string << endl;
}


//////////////////////////////////////////////////////////////////////
// Private
std::optional<std::size_t> Viewmodel::find(int id) {
    for (int i = 0; i < items_.size(); i++) {
        if (items_[i].id == id) {
            return {i};
        }
    }
    return {};
}
