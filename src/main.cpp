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

void initGUI(GLFWwindow *handle, const char *glslVersion = "#version 430") {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(handle, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

bool renderGUI(int& size, int& startPos, int& speed) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static int row = 0;
    static int column = 0;

    ImGui::Begin("Settings");
    if(ImGui::Button("start")) {
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return true;
    }

    ImGui::Text("size");
    ImGui::SliderInt(" ", &size, 5, 13);

    ImGui::Text("start position row");
    ImGui::SliderInt("  ", &row, 0, size - 1, "%d\n");

    ImGui::Text("start position column");
    ImGui::SliderInt("   ", &column, 0, size - 1);
    startPos = size * row + column;
    
    ImGui::Text("speed");
    ImGui::SliderInt("    ", &speed, 1, 10);


    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return false;
}

inline void shutdownGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main(int argc, const char *argv[]) {
    try {
        Window window("ja nie komar", 800, 800);
        initGUI(window);
        int size = 8;
        int startPos = 0;
        int speed = 1;
        
        Renderer tmp(size);
        tmp.setChessBoard(startPos);

        while(true) {
            if(window.shouldClose()) {
                return 0;
            }

            window.clearBuffer();

            tmp.drawChessBoard();
            if(renderGUI(size, startPos, speed) == true) {
                break;
            }

            ImGui::SetWindowPos("Settings", ImVec2(550, 0));
            ImGui::SetWindowSize("Settings", ImVec2(250, 250));
            window.swapBuffers();
            window.pollEvents();
            window.processInput();
        }

        Renderer renderer(size);
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

            if(elapsed.count() >= 600 / speed) {
                window.clearBuffer();

                renderer.drawChessBoard();

                if(i < static_cast<int>(moves.size())) {
                    renderer.drawKnight(moves[i], moves[i + 1]);
                    i += 2;
                }
                else {
                    renderer.drawKnight(0, 0);
                }

                renderGUI(size, startPos, speed);
                begin = end;
            }
            else {
                window.clearBuffer();
                renderer.drawChessBoard();
                renderer.drawKnight(0, 0);
                renderGUI(size, startPos, speed);
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