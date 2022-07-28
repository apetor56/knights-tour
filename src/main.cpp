#include <iostream>
#include <glad.h>
#include "Window.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Data.hpp"

int main(int argc, const char *argv[]) {
    try {
        Window window("ja nie komar", 800, 800);

        VertexArray<> vao;
        
        Buffer<GL_ARRAY_BUFFER, float> vbo;
        vbo.setData(vertices, sizeof(vertices));

        AttributeLayout<float> layout(POSITION | COLOR);
        vbo.vertexAttrib(Position, layout);
        vbo.vertexAttrib(Color, layout);

        Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> ebo;
        ebo.setData(indices, sizeof(indices));

        Shader vertex(GL_VERTEX_SHADER, "../shaders/vertex.glsl");
        Shader fragment(GL_FRAGMENT_SHADER, "../shaders/fragment.glsl");

        unsigned int shaderProgram = Shader::createProgramShader(vertex.getShader(), fragment.getShader());
        Shader::useProgram(shaderProgram);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
        while(!window.shouldClose()) {
            window.clearBuffer();

            window.processInput();
            
            // glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

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