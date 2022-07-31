#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

class Renderer {
public:
    Renderer();

    Renderer(const Renderer& other) = delete;

    Renderer& operator=(const Renderer& other) = delete;

    void setChessBoard(int size = 5);

    void drawChessBoard();

    void generateData(int n);

    void showData() {
        std::cout << "vertices:\n";
        for(int i = 0; i < _size * _size * 12; i+=3) {
            if(i % 4 == 0) {
                std::cout << '\n';
            }
            std::cout << _data[i] << ' ' << _data[i + 1] << ' ' << _data[i + 2] << '\n';
        }

        std::cout << std::string(100, '*') << "\nindexes:\n";
        for(int i = 0; i < _size * _size * 6; i++) {
            if(i % 6 == 0) {
                std::cout << '\n';
            }
            std::cout << _indexes[i] << ' ';
        }
    }

private:
    Buffer<GL_ARRAY_BUFFER, float> _vbo;
    Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> _ebo;
    VertexArray<> _vao;
    Shader _vertex;
    Shader _fragment;
    float *_data;
    unsigned int *_indexes;
    int _size;

    void setSquare(float& x, float& y, float colorFlag) const;

    void setIndexes();
};

#endif // RENDERER_HPP