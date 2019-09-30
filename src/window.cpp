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

Window::Window(const std::string& title, unsigned int width, unsigned int height) 
    : _width{width}, _height{height}
{
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

        // enable and configure depth test
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // enalbe culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* gl_version = glGetString(GL_VERSION);
        const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
        spdlog::info("Loaded OpenGL.");
        spdlog::info("GPU: {} {}", vendor, renderer);
        spdlog::info("OpenGL version: {}", gl_version);
        spdlog::info("GLSL version: {}", glsl_version);

        // register window calbacks
        glfwSetFramebufferSizeCallback( _glfw_window_ptr, Window::_glfw_framebuffer_size_callback );
        glfwSetKeyCallback( _glfw_window_ptr, Window::_glfw_key_callback );
        

        _ref_counter.reset(new bool);
        _window_count++;
        _current_window = this;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::refresh() {
    glfwSwapBuffers(_glfw_window_ptr);
}

void Window::clear_color( float red, float green, float blue, float alpha ) {
    glClearColor( red, green, blue, alpha );
}

std::optional<std::pair<Key, KeyEventType>> Window::next_key() {
    std::optional<std::pair<Key, KeyEventType>> next;
    if( !_key_queue.empty() ) {
        next = _key_queue.front();
        _key_queue.pop();
    }
    return next;
}

/// Window callbacks
void Window::_glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    _current_window->_width = width;
    _current_window->_height = height;
    _current_window->_changed_size = true;
    glViewport( 0, 0, width, height );
}

void Window::_glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    _current_window->_key_queue.push( { (Key)key, (KeyEventType)action } );
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
Window* Window::_current_window = nullptr;
