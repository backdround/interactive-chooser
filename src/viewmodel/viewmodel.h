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

    // Actions
    Q_INVOKABLE void action(QVariant index);
    Q_INVOKABLE void user_input_changed(QVariant input);

private slots:
    void item_inserted(int id);
    void item_erased(int id);

private:
    std::optional<std::size_t> find(int id);

    struct item_t {
        int id;
        QString name;
        QVariant description;
    };
    std::vector<item_t> items_;

    Model_interface& model_;
};
