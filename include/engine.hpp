#ifndef _REPLICATOR_ENGINE_HPP_
#define _REPLICATOR_ENGINE_HPP_

#include "state.hpp"

#include "spdlog/spdlog.h"

#include <string>
#include <cmath>

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
        void set_fov( float fov ) { _fov = fov; }
        void set_near( float near ) { _near = near; }
        void set_far( float far ) { _far = far; }



    private:
        bool _running;
        unsigned int _width, _height;
        std::string _title;
        float _fov =  M_PI / 3.0;
        float _near = -0.1f;
        float _far = -10.0f;

#ifdef DEBUG
        spdlog::level::level_enum _loglevel = spdlog::level::debug;
#else 
        spdlog::level::level_enum _loglevel = spdlog::level::info;
#endif


};

#endif // _REPLICATOR_ENGINE_HPP_
