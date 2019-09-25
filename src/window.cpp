#include "window.hpp"

#include "spdlog/spdlog.h"

#include <stdexcept>

void glfw_error_callback(int code, const char* description);
void GLAPIENTRY opengl_error_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* user_param
);

Window::Window(const std::string& title, unsigned int width, unsigned int height) {
    if( _window_count == 0 ) {
        if(!glfwInit()) {
            const char* description;
            auto code = glfwGetError(&description);
            glfw_error_callback( code, description );
            throw WindowException{"Could not load init GLFW!"};
        }
        spdlog::info("Initialized GLFW.");
        _glfw_window_ptr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!_glfw_window_ptr) 
        {
            const char* description;
            auto code = glfwGetError(&description);
            glfw_error_callback( code, description );
            glfwTerminate();
            throw WindowException{"Could not create window!"};
        }
        spdlog::info("Created window.");

        // register GLFW error callback
        glfwSetErrorCallback( glfw_error_callback );

        glfwMakeContextCurrent(_glfw_window_ptr);
        if( gladLoadGL() == 0 ) {
            glfwTerminate();
            throw WindowException{"Could not load Open GL."};
        }

        // enable and register OpenGL error callback
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback( opengl_error_callback, nullptr );

        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* gl_version = glGetString(GL_VERSION);
        const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
        spdlog::info("Loaded OpenGL.");
        spdlog::info("GPU: {} {}", vendor, renderer);
        spdlog::info("OpenGL version: {}", gl_version);
        spdlog::info("GLSL version: {}", glsl_version);
        

        _ref_counter.reset(new bool);
        _window_count++;
    } else {
        throw std::runtime_error{"Only one window is allowed at the same time."};
    }
}

Window::~Window() {
    if( _ref_counter.unique() ) {
        glfwDestroyWindow(_glfw_window_ptr);
        spdlog::debug("Destroyed window.");
        glfwTerminate();
        spdlog::debug("GLFW terminated.");
        _window_count--;
    }
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::refresh() {
    glfwSwapBuffers(_glfw_window_ptr);
}

void Window::clear_color( float red, float green, float blue, float alpha ) {
    glClearColor( red, green, blue, alpha );
}


/// Error callbacks

void glfw_error_callback(int code, const char* description) {
    spdlog::error("GLFW error code {}: {}", code, description);
}

void GLAPIENTRY opengl_error_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* user_param
        ) {
    switch(type) {
        case GL_DEBUG_TYPE_ERROR:
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            spdlog::error("OpenGL: {}", message);
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        case GL_DEBUG_TYPE_PORTABILITY:
            spdlog::warn("OpenGL: {}", message);
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            spdlog::debug("OpenGL: {}", message);
            break;
        default:
            spdlog::debug("OpenGL: {}", message);
            break;
    }
}


unsigned int Window::_window_count = 0;
