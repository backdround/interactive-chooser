#pragma once
#include <QObject>
#include <QVariant>
#include <QAbstractListModel>

#include <vector>
#include <map>

#include "model_interface.h"

class Viewmodel : public QAbstractListModel {
    Q_OBJECT
public:
    Viewmodel(Model_interface& model);

    int rowCount(const QModelIndex& parent) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex& index, int role) const override;

    enum ROLES {
        ID = Qt::UserRole + 1,
        NAME,
        WEIGHT,
        DESCRIPTION,
    };

    Q_INVOKABLE void action(QVariant id);
    Q_INVOKABLE void user_input_changed(QVariant input);

private slots:
    void item_inserted(int id);
    void item_erased(int id);

private:
    void resort();
    std::string last_input;

    struct item_t {
        int id;
        int weight;
        QString name;
        QVariant description;
        bool operator<(const item_t& lo) const { return weight < lo.weight; }
    };
    std::map<int, item_t> items_;

    std::vector<item_t> sorted_items_;

    Model_interface& model_;
};
