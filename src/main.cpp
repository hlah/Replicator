#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

int main() {
    GLFWwindow* window;


    if(!glfwInit()) {
        std::cerr << "Could not load Open GLFW." << std::endl;
        return -1;
    }

    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window) 
    {
        std::cerr << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL()) {
        std::cerr << "Could not load Open GL." << std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
