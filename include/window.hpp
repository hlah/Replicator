#ifndef _REPLICATOR_WINDOW_HPP_
#define _REPLICATOR_WINDOW_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <memory>

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

        // Return true if window close flag is set
        bool should_close() { return glfwWindowShouldClose(_glfw_window_ptr) == 1; }
        void should_close(bool flag) { return glfwSetWindowShouldClose(_glfw_window_ptr, (int)flag); }

        // Set clear color
        void clear_color( float red, float green, float blue, float alpha=1.0 );

    private:
        GLFWwindow* _glfw_window_ptr;
        std::shared_ptr<bool> _ref_counter;

        static unsigned int _window_count;
};



#endif //  _REPLICATOR_WINDOW_H_
