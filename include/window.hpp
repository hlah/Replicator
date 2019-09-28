#ifndef _REPLICATOR_WINDOW_HPP_
#define _REPLICATOR_WINDOW_HPP_


#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "keys.hpp"

#include <memory>
#include <queue>
#include <optional>

class Window {
    public:
        Window(const std::string& title, unsigned int width, unsigned int height);

        ~Window();

        // Update events
        void poll_events() { glfwPollEvents(); }
        // Clear window
        void clear();
        // Refresh window
        void refresh();

        // Getters and setters

        unsigned int width() const { return _width; }
        unsigned int height() const { return _height; }
        float aspect_ratio() const { return (float)_width/_height; }

        // Return true if window close flag is set
        bool should_close() const { return glfwWindowShouldClose(_glfw_window_ptr) == 1; }
        void should_close(bool flag) { glfwSetWindowShouldClose(_glfw_window_ptr, (int)flag); }

        // Return true if window was resized, reset to false if true 
        bool was_resized_reset() { 
            if( _changed_size ) {
                _changed_size = false;
                return true;
            }
            return false;
        }

        // Set clear color
        void clear_color( float red, float green, float blue, float alpha=1.0 );

        // get next key
        std::optional<std::pair<Key, KeyEventType>> next_key();

    private:
        GLFWwindow* _glfw_window_ptr;
        std::shared_ptr<bool> _ref_counter;

        unsigned int _width, _height;
        bool _changed_size = true;

        static unsigned int _window_count;
        static Window* _current_window;

        // event queues
        std::queue<std::pair<Key, KeyEventType>> _key_queue;
        
        // callbacks
        static void _glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void _glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

class WindowException : public std::exception {
    public:
        WindowException( const std::string& msg ) : _msg{msg} {}
        const char* what() const throw() { return _msg.c_str(); }
    private:
        const std::string _msg;
};

#endif //  _REPLICATOR_WINDOW_H_
