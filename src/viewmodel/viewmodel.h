#pragma once
#include <QObject>
#include <QVariant>
#include <QAbstractListModel>

#include <vector>

#include "model_interface.h"

class Viewmodel : public QAbstractListModel {
    Q_OBJECT
public:
    Viewmodel(Model_interface& model);

    int rowCount(const QModelIndex& parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex& index, int role) const override;

    enum ROLES {
        NAME = Qt::UserRole + 1,
        WEIGHT,
        DESCRIPTION,
    };

    Q_INVOKABLE void action(QVariant index);

private slots:
    void item_inserted(std::size_t index);
    void item_erased(std::size_t index);

private:
    struct item_t {
        QString name;
        int weight;
        QString description;
    };
    std::vector<item_t> items_;

    Model_interface& model_;
};
