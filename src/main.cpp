#include <iostream>

#include "engine.hpp"

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    //Scene main_scene{}
    //scene.add_object(...);

    //engine.set_scene(main_scene);

    engine.run();
    
    return 0;
}
