#pragma once
#include <QObject>

#include <string>
#include <optional>


struct item_declaration_t {
    const bool id = true;
    const bool name = true;

    bool description = false;
};

struct item_t {
    int id;
    std::string name;

    std::optional<std::string> description;
};


class Model_interface : public QObject {
    Q_OBJECT

public:
    virtual std::list<item_t> items() = 0;
    virtual std::optional<item_t> item(int id) = 0;

    virtual void action(int id) = 0;
    virtual void user_input_changed(std::string input) = 0;

    virtual item_declaration_t declaration() { return {}; }

signals:
    void item_inserted(int id);
    void item_erased(int id);
};
