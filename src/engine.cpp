#include "engine.hpp"

#include "window.hpp"
#include "state.hpp"
#include "object.hpp"
#include "matrix_op.hpp"

void Engine::run(State* state_ptr) {
    _running = true;

    spdlog::set_level(_loglevel);

    Window window{ _title, _width, _height };

    spdlog::info("Running!");

    World world{};

    state_ptr->on_start(world);
    while( _running ) {
        window.poll_events();

        if( window.was_resized_reset() ) {
            world.set_projection_matrix( 
                    matrix_op::perspective(
                    _fov, window.aspect_ratio(),
                    _near, _far
                )
            );
        }

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

        window.clear();
        world.draw();
        window.refresh();
    }
    spdlog::info("Stoped.");
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
