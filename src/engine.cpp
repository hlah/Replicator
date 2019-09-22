#include "engine.hpp"

#include "window.hpp"
#include "state.hpp"

#include <iostream>

void Engine::run(State* state_ptr) {
    _running = true;
    Window window{ _title, _width, _height };

    while( _running ) {
        auto transition = State::Transition::NONE;
        if( window.should_close() ) {
            transition = state_ptr->on_close();
            window.should_close(false);
        }

        if( transition == State::Transition::QUIT ) {
            stop();
            break;
        }

        state_ptr->update();

        window.poll_events();
    }
}

void Engine::stop() {
    _running = false;
}

void Engine::set_window_size(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
}

void Engine::set_window_title(const std::string& title) {
    _title = title;
}
