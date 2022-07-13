#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glfw3.h>
#include "Exception.hpp"

class Window {
public:
    explicit Window(const char *name, int width, int height);

    Window(const Window& other) = delete;

    ~Window();

    void processInput();

    int shouldClose() noexcept;

    void swapBuffers();

    void pollEvents();

private:
    int _width;
    int _height;
    const char *_name;
    GLFWwindow *windowHandle;
    
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

class WindowException : public Exception {
public:
    using Exception::Exception;

    const char *getType() const override;

    const char *what() const noexcept override;
};

#endif // WINDOW_HPP