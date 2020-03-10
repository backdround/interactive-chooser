#pragma once
#include "model_interface.h"

class Model_factory {
public:
    Model_factory(int argc, char* argv[]);

    Model_interface* create();
};
