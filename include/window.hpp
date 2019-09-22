#ifndef _REPLICATOR_WINDOW_HPP_
#define _REPLICATOR_WINDOW_HPP_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <memory>

class Window {
    public:
        Window(const std::string& title, unsigned int width, unsigned int height);

        ~Window();
        void foo() {}

    private:
        GLFWwindow* _glfw_window_ptr;
        std::shared_ptr<bool> _ref_counter;

        static unsigned int _window_count;
};



#endif //  _REPLICATOR_WINDOW_H_
