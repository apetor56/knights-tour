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

    void setChessBoard(int size = 5, int startPos = 0);

    void drawChessBoard() const;

    void drawKnight(int move_x, int move_y);

private:
    // buffers, VAO and shaders for chessboard
    Buffer<GL_ARRAY_BUFFER, float> _vboChess;
    Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> _eboChess;
    VertexArray _vaoChess;
    Shader _vertexChess;
    Shader _fragmentChess;
    unsigned int _programChess;

    // buffers, VAO and shaders for chessboard
    Buffer<GL_ARRAY_BUFFER, float> _vboKnight;
    Buffer<GL_ELEMENT_ARRAY_BUFFER, unsigned int> _eboKnight;
    VertexArray _vaoKnight;
    Shader _vertexKnight;
    Shader _fragmentKnight;
    unsigned int _programKnight;

    float *_chessData;
    unsigned int *_indexes;
    int _size;
    int _startPos;

    void generateData(int n);

    void setSquare(float& x, float& y, float colorFlag) const;

    void setKnightData(float start_x, float start_y, float length);

    void setIndexes();
};

#endif // RENDERER_HPP