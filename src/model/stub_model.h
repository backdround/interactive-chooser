#pragma once
#include <map>

#include "model_interface.h"

class Stub_model : public Model_interface {
public:
    Stub_model();

    std::list<item_t> items() override;
    std::optional<item_t> item(int id) override;

    void action(int id) override;
    void user_input_changed(std::string input) override;

private:
    std::map<int, item_t> items_;
};
