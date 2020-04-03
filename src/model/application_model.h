#pragma once
#include <map>
#include <filesystem>

#include "model_interface.h"



class Application_model : public Model_interface {
public:
    Application_model();

    std::list<item_t> items() override;
    std::optional<item_t> item(int id) override;

    void action(int id) override;
    int calculate_weight(int id, const std::string& sort_string) override;

private:
    struct extended_item_t : public item_t {
        std::filesystem::path path;
    };

    std::map<int, extended_item_t> items_;
};
