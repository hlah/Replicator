#include <iostream>

#include "engine.hpp"
#include "state.hpp"

class MyState : public State {
    public:
        virtual Transition on_close() { 
            if( _counter > 0 ) {
                std::cout << "Closing in " << _counter << std::endl;
                _counter--;
                return State::Transition::NONE; 
            }
            return State::Transition::QUIT; 
        }
    private:
        int _counter = 3;
};

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");

    MyState state;

    engine.run(&state);
    
    return 0;
}
