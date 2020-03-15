#pragma once
#include <vector>
#include <string>

#include "model_interface.h"

class Stub_model : public Model_interface {
public:
    Stub_model();
    void user_input_changed(std::string input) override;

    std::size_t size() override;

    std::string name(std::size_t i) override;
    int weight(std::size_t i) override;

    void action(std::size_t i) override;

private:
    std::vector<std::string> items_;
};
