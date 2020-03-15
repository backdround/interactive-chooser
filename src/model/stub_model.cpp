#include "stub_model.h"

#include <iostream>
using namespace std;

Stub_model::Stub_model() {
    items_.push_back("one");
    items_.push_back("two");
    items_.push_back("tree");
    items_.push_back("four");
}

void Stub_model::user_input_changed(std::string input) {

}

std::size_t Stub_model::size() {
    return items_.size();
}

std::string Stub_model::name(std::size_t i) {
    return items_[i];
}

int Stub_model::weight(std::size_t i) {
    return 1;
}

void Stub_model::action(std::size_t i) {
    cout << "action index: " << i << endl;
}
