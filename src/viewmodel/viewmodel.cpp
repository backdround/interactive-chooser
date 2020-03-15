#include <QDebug>

#include "viewmodel.h"

Viewmodel::Viewmodel(Model_interface& model) : model_(model) {
    connect(&model_, &Model_interface::item_inserted, this,
            &Viewmodel::item_inserted);
    connect(&model_, &Model_interface::item_erased, this,
            &Viewmodel::item_erased);

    for (int i = 0; i < model_.size(); i++) {
        item_t item;
        item.name = model_.name(i).c_str();
        item.weight = model_.weight(i);
        item.description = model_.description(i).c_str();
        items_.push_back(item);
    }
}

int Viewmodel::rowCount(const QModelIndex& parent) const {
    return items_.size();
}

QHash<int, QByteArray> Viewmodel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NAME] = "name";
    roles[WEIGHT] = "weight";
    if (model_.use_description()) {
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
        case NAME:
            return {items_[index].name};
        case WEIGHT:
            return {items_[index].weight};
        case DESCRIPTION:
            return {items_[index].description};
        default:
            return {};
    }
}

void Viewmodel::action(QVariant index) {
    if (!index.canConvert<std::size_t>()) {
        qWarning() << "Couldn't convert action index to size_t";
    }
    auto i = index.value<std::size_t>();
    model_.action(i);
}


void Viewmodel::item_inserted(std::size_t index) {
    item_t item;
    item.name = model_.name(index).c_str();
    item.weight = model_.weight(index);
    item.description = model_.description(index).c_str();

    beginInsertRows({}, index, index);
    items_.insert(items_.begin() + index, item);
    endInsertRows();
}

void Viewmodel::item_erased(std::size_t index) {
    auto erased_item = items_.begin() + index;
    beginRemoveRows({}, index, index);
    items_.erase(erased_item);
    endRemoveRows();
}
