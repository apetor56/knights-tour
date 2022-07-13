#include <iostream>
#include "Window.hpp"

int main(int argc, const char *argv[]) {
    try {
        Window window("ja nie komar", 800, 600);

        while(!window.shouldClose()) {
            window.processInput();

            window.swapBuffers();

            window.pollEvents();
        }
    }
    catch(Exception& e) {
        std::cout << e.what() << '\n';
    }
    catch(std::exception& e) {
        std::cout << e.what() << '\n';
    }
    catch(...) {
        std::cout << "undefined exception\n";
    }
}