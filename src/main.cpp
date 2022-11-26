#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Renderer.hpp"
#include "Texture2D.hpp"
#include "Window.hpp"
#include "KnightsTour.hpp"

void renderGUI(int& size, int& startPos) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static int row = 0;
    static int column = 0;

    ImGui::Begin("Settings");
    if(ImGui::Button("start")) {
        std::cout << "nacisnieto\n";
    }

    ImGui::Text("size");
    ImGui::SliderInt(" ", &size, 5, 13);

    ImGui::Text("start position row");
    ImGui::SliderInt("  ", &row, 0, size - 1, "%d\n");

    ImGui::Text("start position column");
    ImGui::SliderInt("   ", &column, 0, size - 1);
    startPos = size * column + row;
    
    int speed;
    ImGui::Text("speed");
    ImGui::SliderInt("    ", &speed, 0, 10);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdownGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main(int argc, const char *argv[]) {
    try {
        Window window("ja nie komar", 800, 800);
        const char *glsl_version = "#version 430";

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        
        if(argc != 3) {
            std::cerr << "Wrong number of arguments\n";
            return -1;
        }

        int size = atoi(argv[1]);
        int startPos = atoi(argv[2]);
        Renderer renderer(size);

        // renderer.setChessBoard(startPos);
        // window.clearBuffer();
        // window.pollEvents();
        // window.processInput();
        // renderer.drawChessBoard();


        // window.swapBuffers();

        // while(true);

        KnightTour knight(size);
        bool *visited = new bool[size * size]();
        knight.findSolution(startPos, visited, std::vector<int>({startPos}));

        std::vector<int> res = knight.getResult();
        if(res.size() == 0) {
            return 1;
        }

        std::vector<int> moves = knight.convertOutput();

        renderer.setChessBoard(startPos);

        auto begin = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

        int i = 0;
        while(!window.shouldClose()) {
            end = std::chrono::high_resolution_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

            if(elapsed.count() >= 600) {
                window.clearBuffer();

                renderer.drawChessBoard();

                if(i < static_cast<int>(moves.size())) {
                    renderer.drawKnight(moves[i], moves[i + 1]);
                    i += 2;
                }
                else {
                    renderer.drawKnight(0, 0);
                }

                renderGUI(size, startPos);
                begin = end;
            }
            else {
                window.clearBuffer();
                renderer.drawChessBoard();
                renderer.drawKnight(0, 0);
                renderGUI(size, startPos);
                window.swapBuffers();
                window.pollEvents();
                window.processInput();
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