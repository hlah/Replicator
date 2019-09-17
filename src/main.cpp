#include <iostream>

#include "window.hpp"

int main() {
    auto window = Window{"Test", 800, 800};
    window.foo();

    auto window2 = window;
    window2.foo();


    return 0;
}
