#ifndef _REPLICATOR_ENGINE_HPP_
#define _REPLICATOR_ENGINE_HPP_

#include "state.hpp"

#include <string>

class Engine {
    public:
        // Constructor
        Engine() : _running{false}, _width{512}, _height{512}, _title{} {}

        // run engine
        void run(State* state_ptr);

        // stop engine
        void stop();

        // Setters
        void set_window_size(unsigned int width, unsigned int height);
        void set_window_title(const std::string& title);


    private:
        bool _running;
        unsigned int _width;
        unsigned int _height;
        std::string _title;
};

#endif // _REPLICATOR_ENGINE_HPP_
