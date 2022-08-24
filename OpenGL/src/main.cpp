#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "Window.hpp"

int main(int argc, const char *argv[]) {
    try {
        Window window("ja nie komar", 800, 800);

        if(argc != 3) {
            std::cerr << "Wrong number of arguments\n";
            return -1;
        }

        int size = atoi(argv[1]);
        int startPos = atoi(argv[2]);
        Renderer renderer(size);

        renderer.setChessBoard(startPos);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        int x = 0, old_x = 0;
        int y = 0, old_y = 0;
        int i = 0;
        while(!window.shouldClose()) {
            window.clearBuffer();

            window.processInput();
            
            renderer.drawChessBoard();

            if(i < size * size) {
                renderer.drawKnight(x, y);
                std::cin >> x >> y;
                i++;
            }
            else {
                renderer.drawKnight(old_x, old_y);
            }

            window.swapBuffers();
            window.pollEvents();

            if(i < size * size) {
                std::this_thread::sleep_for(std::chrono::milliseconds(600));
            }
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