#include "engine.hpp"

#include "state.hpp"
#include "object.hpp"
#include "matrix_op.hpp"

void Engine::run(State* state_ptr) {
    _running = true;

    spdlog::set_level(_loglevel);

    Window window{ _title, _width, _height };

    spdlog::info("Running!");

    World world{};
    
    state_ptr->on_start( world );

    while( _running ) {
        window.poll_events();

        _process_actions( window, state_ptr );

        // update projection matrix on window resize
        if( window.was_resized_reset() ) {
            world.set_projection_matrix( 
                    matrix_op::perspective(
                    _fov, window.aspect_ratio(),
                    _near, _far
                )
            );
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

ActionId Engine::get_action_id( const std::string& name ) {
    auto action_it = std::find_if( _actions.begin(), _actions.end(),
            [&name] (auto it) { return (it.second == name); }
    );
    if( action_it == _actions.end() ) {
        _actions[_next_action_id] = name;
        return _next_action_id++;
    }
    else {
        return action_it->first;
    }
}

void Engine::bind_key( Key key, ActionId action ) {
    _key_bindings[key] = action;
}

void Engine::_process_actions( Window& window, State* state_ptr ) {
    // check for close event
    if( window.should_close() ) {
        auto transition = state_ptr->on_close();
        window.should_close(false);
        _process_transition( transition );
    }

    // process input events
    std::optional<std::pair<Key, KeyEventType>> key_event;
    while( (key_event = window.next_key()) ) {
        if( _key_bindings.count( key_event->first ) != 0 ) {
            // Key press -> action ON
            if( key_event->second == KeyEventType::Press ) {
                auto transition = state_ptr->on_action( 
                        ActionEvent{ ActionEvent::Type::ON, _actions.at( _key_bindings.at( key_event->first ) ) } 
                        );
                _process_transition( transition );
            } 
            // Key release -> action OFF
            else if( key_event->second == KeyEventType::Release ) {
                auto transition = state_ptr->on_action( 
                    ActionEvent{ ActionEvent::Type::OFF, _actions.at( _key_bindings.at( key_event->first ) ) } 
                );
                _process_transition( transition );
            } 
        }
    }
}

void Engine::_process_transition( State::Transition trans ) {
    switch( trans ) {
        case State::Transition::QUIT:
            stop();
            break;
        case State::Transition::NONE:
            break;
    }
}
