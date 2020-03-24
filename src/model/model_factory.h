#pragma once
#include "model_interface.h"

#include <optional>
#include <string>

class Model_factory {
public:
    Model_factory(int argc, char* argv[]);

    Model_interface* create();

    std::optional<std::string> error_string();

private:
    std::optional<std::string> error_string_;

    enum MODE {
        STUB,
        APPLICATION
    };
    MODE mode_ = STUB;
};
