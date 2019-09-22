#include "window.hpp"

#include <stdexcept>

#include <iostream>

Window::Window(const std::string& title, unsigned int width, unsigned int height) {
    if( _window_count == 0 ) {
        if(!glfwInit()) {
            throw std::runtime_error{"Could not load init GLFW!"};
        }
        _glfw_window_ptr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_glfw_window_ptr) 
        {
            glfwTerminate();
            throw std::runtime_error{"Could not create window!"};
        }
        glfwMakeContextCurrent(_glfw_window_ptr);
        if(!gladLoadGL()) {
            glfwTerminate();
            throw std::runtime_error{"Could not load Open GL."};
        }

        _ref_counter.reset(new bool);
        _window_count++;
    } else {
        throw std::runtime_error{"Only one window is allowed at the same time."};
    }
}

Window::~Window() {
    if( _ref_counter.unique() ) {
        glfwDestroyWindow(_glfw_window_ptr);
        glfwTerminate();
        _window_count--;
    }
}

unsigned int Window::_window_count = 0;
