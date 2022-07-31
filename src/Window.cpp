#include <sstream>
#include <glad.h>
#include "Window.hpp"

#define WND_EXC WindowException(__LINE__, __FILE__)

//-------------------------------------------------------------//
//---------------- Window class implementation ----------------//
//-------------------------------------------------------------//

Window::CursorPos Window::mouse = {0.0, 0.0};

Window::Window(const char *name, int width = 800, int height = 600)
:   _name(name), _width(width), _height(height)
{
    // initialize glfw framework
    if(glfwInit() == GLFW_FALSE) {
        throw WND_EXC;
    }

    // set GLFW context version 3.3 with core profile
    // (without OpenGL legacy)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    if(!(windowHandle = glfwCreateWindow(width, height, name, nullptr, nullptr))) {
        throw WND_EXC;
    }

    // set current context for 'windowHandle'
    // (this will remember STATE MACHINE (like binded textures, buffers ect.)
    // for this specific window)
    glfwMakeContextCurrent(windowHandle);

    // change viewport setting while resizing window event
    glfwSetFramebufferSizeCallback(windowHandle, framebuffer_size_callback);

    glfwSetCursorPosCallback(windowHandle, cursor_position_callback);

    // manage access for OpelGL functions' location (memory)
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw WND_EXC;
    }

    /* std::cout << glGetString(GL_VERSION) << '\n'
    << glGetString(GL_RENDERER) << '\n'
    << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n'; */
}

// end using glfw
Window::~Window() {
    glfwTerminate();
}

// Callback function for resizing window, change viewport settings.
void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouse.x = xpos;
    mouse.y = ypos;
}

// Checks input from mouse and keyboard.
void Window::processInput() {
    if(glfwGetKey(windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(windowHandle, true);
    }

    int state = glfwGetMouseButton(windowHandle, GLFW_MOUSE_BUTTON_LEFT);
    if(state == GLFW_PRESS) {
        std::cout << '[' << mouse.x << ',' << mouse.y << "]\n";
    }
}

// Checks if window got event called 'glfwSetWindowShouldClose'.
int Window::shouldClose() noexcept {
    return glfwWindowShouldClose(windowHandle);
}

// Swap back and front buffers.
void Window::swapBuffers() {
    glfwSwapBuffers(windowHandle);
}

// Processes all pending events.
void Window::pollEvents() {
    glfwPollEvents();
}

// clears frontbuffer at the beginning of new frame
void Window::clearBuffer() {
    glClearColor(0.3f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

const char *Window::getName() const noexcept {
    return _name;
}

Window::resolution Window::getResolution() const noexcept {
    return {_width, _height};
}

//-----------------------------------------------------------------//
//---------------- Window Exception implementation ----------------//
//-----------------------------------------------------------------//

const char *WindowException::getType() const {
    return "Window Exception";
}

const char *WindowException::what() const noexcept {
    std::ostringstream oss;
    oss << "[FILE] " << getFile() << ":" << getLine() << '\n'
        << "[LINE] " << getLine() << '\n'
        << "[TYPE] " << getType();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}