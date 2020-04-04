#include <string>

#include <cxxopts.hpp>

#include "stub_model.h"
#include "application_model.h"

#include "model_factory.h"

Model_factory::Model_factory(int argc, char* argv[]) {
    try {
        cxxopts::Options options("chooser", "Convenient way to choose anythin");
        options.add_options()(
            "m,mode", "Use mode (application|stub)",
            cxxopts::value<std::string>()->default_value("stub"));

        auto result = options.parse(argc, argv);

        // Parse mode.
        auto mode = result["mode"].as<std::string>();
        if (mode == "stub") {
            mode_ = STUB;
        } else if (mode == "application") {
            mode_ = APPLICATION;
        } else {
            throw cxxopts::OptionSpecException("Unknown mode");
        }

    } catch (cxxopts::OptionException& exc) {
        error_string_ = exc.what();
    }
}

Model_interface* Model_factory::create() {
    if (error_string_) {
        return nullptr;
    }

    switch (mode_) {
        case APPLICATION:
            return new Application_model;
        case STUB:
        default:
            return new Stub_model;
    }
}

std::optional<std::string> Model_factory::error_string() {
    return error_string_;
}
