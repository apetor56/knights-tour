#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glfw3.h>
#include "Exception.hpp"

class Window {
private:
    struct resolution {
        int x;
        int y;
    };

public:
    explicit Window(const char *name, int width, int height);

    Window(const Window& other) = delete;

    ~Window();

    void processInput();

    int shouldClose() noexcept;

    void swapBuffers();

    void pollEvents();

    void clearBuffer();

    const char *getName() const noexcept;

    resolution getResolution() const noexcept;
    
private:
    const char *_name;
    int _width;
    int _height;
    GLFWwindow *windowHandle;

    static struct CursorPos {
        double x;
        double y;
    } mouse;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
};

class WindowException : public Exception {
public:
    using Exception::Exception;

    const char *getType() const override;

    const char *what() const noexcept override;
};

#endif // WINDOW_HPP