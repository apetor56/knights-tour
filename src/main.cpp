#include <iostream>
#include <vector>
#include "Renderer.hpp"
#include "Window.hpp"

int main(int argc, const char *argv[]) {
    try {
        Window window("ja nie komar", 600, 600);
        Renderer renderer;
    
        if(argc != 2) {
            return -1;
        }
        renderer.setChessBoard(std::stoi(argv[1]));
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
        while(!window.shouldClose()) {
            window.clearBuffer();

            window.processInput();
            
            renderer.drawChessBoard();

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