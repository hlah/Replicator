#include "engine.hpp"

#include "state.hpp"
#include "object.hpp"
#include "matrix_op.hpp"

#include "entt/entt.hpp"

void Engine::run(State* state_ptr) {
    _running = true;

    spdlog::set_level(_loglevel);

    auto window = std::make_shared<Window>( _title, _width, _height );
    entt::registry registry;
    registry.set<std::shared_ptr<Window>>( window );

    spdlog::info("Running!");
    
    state_ptr->on_start( registry );

    double before = glfwGetTime();
    while( _running ) {
        window->poll_events();

        _process_actions( registry, state_ptr, *window );

        window->clear();
        double now = glfwGetTime();
        state_ptr->update( registry );
        before = now;

        window->refresh();
        window->reset();
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

void Engine::_process_actions( entt::registry& registry, State* state_ptr, Window& window ) {
    // check for close event
    if( window.should_close() ) {
        auto transition = state_ptr->on_close( registry );
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
                        registry,
                        ActionEvent{ ActionEvent::Type::ON, _actions.at( _key_bindings.at( key_event->first ) ) }
                );
                _process_transition( transition );
            } 
            // Key release -> action OFF
            else if( key_event->second == KeyEventType::Release ) {
                auto transition = state_ptr->on_action( 
                        registry,
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
