#include "engine.hpp"

#include "window.hpp"

void Engine::run() {
    _running = true;
    Window window{ _title, _width, _height };

    while( _running ) {

    }
}

void Engine::set_window_size(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
}

void Engine::set_window_title(const std::string& title) {
    _title = title;
}
