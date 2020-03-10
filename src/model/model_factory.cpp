#include "stub_model.h".h"

#include "model_factory.h"

Model_factory::Model_factory(int argc, char* argv[]) {

}

Model_interface* Model_factory::create() {
    return new Stub_model;
}
