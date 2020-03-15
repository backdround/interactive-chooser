#pragma once
#include <QObject>

#include <string>

class Model_interface : public QObject{
    Q_OBJECT

public:
    virtual void user_input_changed(std::string input) {};

    virtual std::size_t size() = 0;

    virtual std::string name(std::size_t i) = 0;
    virtual int weight(std::size_t i) = 0;

    virtual void action(std::size_t i) = 0;

    virtual bool use_description() { return false; };
    virtual std::string description(std::size_t i) { return {}; };

signals:
    void item_inserted(std::size_t index);
    void item_erased(std::size_t index);
};
